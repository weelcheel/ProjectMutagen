#include "DigitalForge.h"
#include "DFWeapon.h"
#include "DigitalForgeCharacter.h"
#include "UnrealNetwork.h"

ADFWeapon::ADFWeapon(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	WeaponMesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("WeaponMesh"));
	WeaponMesh->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	WeaponMesh->bChartDistanceFactor = true;
	WeaponMesh->bReceivesDecals = false;
	WeaponMesh->CastShadow = true;
	WeaponMesh->SetCollisionObjectType(ECC_WorldDynamic);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	RootComponent = WeaponMesh;

	bPlayingAttackAnim = false;
	bIsEquipped = false;
	bWantsToAttack = false;
	bPendingEquip = false;
	CurrentState = EWeaponState::Idle;

	LastAttackTime = 0.0f;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;
	SetRemoteRoleForBackwardsCompat(ROLE_SimulatedProxy);
	bReplicates = true;
	bReplicateInstigator = true;
	bNetUseOwnerRelevancy = true;
}

void ADFWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	DetachMeshFromPawn();
}

void ADFWeapon::OnEquip()
{
	AttachMeshToPawn();

	bPendingEquip = true;
	DetermineWeaponState();

	float duration = PlayWeaponAnimation(EquipAnimation);
	if (duration <= 0.0f)
	{
		duration = 0.5f;
	}
	EquipStartedTime = GetWorld()->GetTimeSeconds();
	EquipDuration = duration;

	GetWorldTimerManager().SetTimer(this, &ADFWeapon::OnEquipFinished, duration, false);

	if (MyPawn && MyPawn->IsLocallyControlled())
	{
		//play weapon equip sound
	}
}

void ADFWeapon::OnEquipFinished()
{
	AttachMeshToPawn();

	bIsEquipped = true;
	bPendingEquip = false;

	DetermineWeaponState();
}

void ADFWeapon::OnUnEquip()
{
	DetachMeshFromPawn();
	bIsEquipped = false;
	StopAttack();

	if (bPendingEquip)
	{
		StopWeaponAnimation(EquipAnimation);
		bPendingEquip = false;

		GetWorldTimerManager().ClearTimer(this, &ADFWeapon::OnEquipFinished);
	}

	DetermineWeaponState();
}

void ADFWeapon::OnEnterInventory(ADigitalForgeCharacter* NewOwner)
{
	SetOwningPawn(NewOwner);
}

void ADFWeapon::OnLeaveInventory()
{
	if (Role == ROLE_Authority)
	{
		SetOwningPawn(NULL);
	}

	if (IsAttachedToPawn())
	{
		OnUnEquip();
	}
}

void ADFWeapon::AttachMeshToPawn()
{
	if (MyPawn)
	{
		DetachMeshFromPawn();

		FName AttachPoint; //= GetWeaponAttachPoint()
		USkeletalMeshComponent* pawnmesh = MyPawn->GetMesh();
		WeaponMesh->AttachTo(pawnmesh, AttachPoint);
	}
}

void ADFWeapon::DetachMeshFromPawn()
{
	WeaponMesh->DetachFromParent();
	WeaponMesh->SetHiddenInGame(true);

	//@todo: implement holstering
}

void ADFWeapon::StartAttack()
{
	if (Role < ROLE_Authority)
	{
		ServerStartAttack();
	}

	if (!bWantsToAttack)
	{
		bWantsToAttack = true;
		DetermineWeaponState();
	}
}

void ADFWeapon::StopAttack()
{
	if (Role < ROLE_Authority)
	{
		ServerStopAttack();
	}

	if (bWantsToAttack)
	{
		bWantsToAttack = false;
		DetermineWeaponState();
	}
}

bool ADFWeapon::ServerStartAttack_Validate()
{
	return true;
}

void ADFWeapon::ServerStartAttack_Implementation()
{
	StartAttack();
}

bool ADFWeapon::ServerStopAttack_Validate()
{
	return true;
}

void ADFWeapon::ServerStopAttack_Implementation()
{
	StopAttack();
}

bool ADFWeapon::CanAttack() const
{
	bool bCanAttack = false;

	if (MyPawn)
		bCanAttack = true;

	bool bStatesOK = ((CurrentState == EWeaponState::Idle) || (CurrentState == EWeaponState::Attacking));

	return (bCanAttack && bStatesOK);
}

void ADFWeapon::HandleAttack()
{
	if (CanAttack())
	{
		if (MyPawn && MyPawn->IsLocallyControlled())
		{
			WeaponAttack();

			if (Role < ROLE_Authority)
			{
				ServerHandleAttack();
			}

			//@todo: setup looping attacks
		}
	}

	LastAttackTime = GetWorld()->GetTimeSeconds();
}

bool ADFWeapon::ServerHandleAttack_Validate()
{
	return true;
}

void ADFWeapon::ServerHandleAttack_Implementation()
{
	HandleAttack();
}

void ADFWeapon::SetWeaponState(EWeaponState::Type NewState)
{
	const EWeaponState::Type PrevState = CurrentState;

	if (PrevState == EWeaponState::Attacking && NewState != EWeaponState::Attacking)
	{
		OnAttackFinished();
	}

	CurrentState = NewState;

	if (PrevState != EWeaponState::Attacking && NewState == EWeaponState::Attacking)
	{
		OnAttackStarted();
	}
}

void ADFWeapon::DetermineWeaponState()
{
	EWeaponState::Type NewState = EWeaponState::Idle;

	if (bIsEquipped)
	{
		if (bWantsToAttack && CanAttack())
		{
			NewState = EWeaponState::Attacking;
		}
	}
	else if (bPendingEquip)
	{
		NewState = EWeaponState::Equipping;
	}

	SetWeaponState(NewState);
}

void ADFWeapon::OnAttackStarted()
{
	HandleAttack();
	PlayWeaponAnimation(AttackAnimation);
}

void ADFWeapon::OnAttackFinished()
{
	bReattacking = false;
}

UAudioComponent* ADFWeapon::PlayWeaponSound(USoundCue* Sound)
{
	UAudioComponent* AC = NULL;
	if (Sound && MyPawn)
	{
		AC = UGameplayStatics::PlaySoundAttached(Sound, MyPawn->GetRootComponent());
	}

	return AC;
}

float ADFWeapon::PlayWeaponAnimation(UAnimMontage* Animation)
{
	float duration = 0.0f;
	if (MyPawn && Animation)
	{
		duration = MyPawn->PlayAnimMontage(Animation);
	}

	return duration;
}

void ADFWeapon::StopWeaponAnimation(UAnimMontage* Animation)
{
	if (MyPawn && Animation)
	{
		MyPawn->StopAnimMontage(Animation);
	}
}

FHitResult ADFWeapon::WeaponTrace(const FVector& TraceFrom, const FVector& TraceTo) const
{
	static FName WeaponAttackTag = FName(TEXT("WeaponTrace"));

	FCollisionQueryParams TraceParams(WeaponAttackTag, true, Instigator);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;

	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingle(Hit, TraceFrom, TraceTo, ECollisionChannel::ECC_Vehicle, TraceParams);

	return Hit;
}

void ADFWeapon::SetOwningPawn(ADigitalForgeCharacter* DFCharacter)
{
	if (MyPawn != DFCharacter)
	{
		Instigator = DFCharacter;
		MyPawn = DFCharacter;
		SetOwner(DFCharacter);
	}
}

void ADFWeapon::OnRep_MyPawn()
{
	if (MyPawn)
	{
		OnEnterInventory(MyPawn);
	}
	else
	{
		OnLeaveInventory();
	}
}

void ADFWeapon::GetLifetimeReplicatedProps( TArray< class FLifetimeProperty > & OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADFWeapon, MyPawn);
}

ADigitalForgeCharacter* ADFWeapon::GetPawnOwner() const
{
	return MyPawn;
}

bool ADFWeapon::IsEquipped() const
{
	return bIsEquipped;
}

bool ADFWeapon::IsAttachedToPawn() const
{
	return bIsEquipped || bPendingEquip;
}

EWeaponState::Type ADFWeapon::GetCurrentState() const
{
	return CurrentState;
}

float ADFWeapon::GetEquipStartedTime() const
{
	return EquipStartedTime;
}

float ADFWeapon::GetEquipDuration() const
{
	return EquipDuration;
}

USkeletalMeshComponent* ADFWeapon::GetWeaponMesh() const
{
	return WeaponMesh;
}

bool ADFWeapon::GetWeaponWantsToAttack()
{
	return bWantsToAttack;
}