#include "Asteroid.h"
#include "AIController.h"
#include "Spaceship.h"
#include "Projectile.h"

#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/CapsuleComponent.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystem.h"

AAsteroid::AAsteroid()
{
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collider"));
	Collider->SetCapsuleHalfHeight(22.0f);
	Collider->SetCapsuleRadius(20.0f);
	Collider->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	RootComponent = Collider;

	SM_Asteroid = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AsteroidStaticMesh"));
	SM_Asteroid->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
	SM_Asteroid->SetEnableGravity(false);
	SM_Asteroid->SetConstraintMode(EDOFMode::XZPlane);
	SM_Asteroid->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SM_Asteroid->SetCollisionProfileName(TEXT("CharacterMesh"));
	SM_Asteroid->SetupAttachment(RootComponent);

	FloatingMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingMovement"));
	FloatingMovement->Acceleration = 1000.0f;

	PitchValue = 0.0f;
	YawValue = 0.0f;
	RollValue = 0.0f;

	Tags.AddUnique(TEXT("Asteroid"));

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AAsteroid::BeginPlay()
{
	Super::BeginPlay();

	Collider->OnComponentBeginOverlap.AddDynamic(this, &AAsteroid::OnOverlapBegin);

	FollowPlayerLocation = FMath::RandBool();
	FloatingMovement->MaxSpeed = (FollowPlayerLocation) ? 300 : 0;

	SetLifeSpan(10.0f);
}

void AAsteroid::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndexType, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Projectile"))
	{
		AProjectile* Projectile = Cast<AProjectile>(OtherActor);

		Projectile->Destroy();

		UGameplayStatics::PlaySound2D(this, DestroySound);

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, this->GetActorLocation(), FRotator::ZeroRotator);

		this->Destroy();
	}
}

void AAsteroid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AIController == nullptr)
		AIController = Cast<AAIController>(GetController());

	if (PlayerActor == nullptr)
		FindPlayer();

	if (AIController != nullptr)
		MoveTowardPlayer();

	RotateAsteroid();
}

void AAsteroid::FindPlayer()
{
	TArray<AActor*> Players;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpaceship::StaticClass(), Players);

	if (Players.Num() > 0)
	{
		PlayerActor = Players[0];
		MovementDestination = PlayerActor->GetActorLocation();
	}
	else
		MovementDestination = FVector(0.f, 0.f, 0.f);
}

void AAsteroid::MoveTowardPlayer()
{
	AIController->MoveToLocation(MovementDestination, -1.0f, true, false, false, false, 0, false);
}

void AAsteroid::RotateAsteroid()
{
	PitchValue = FMath::RandRange(0.5f, 2.0f);
	YawValue = FMath::RandRange(0.5f, 2.0f);
	RollValue = FMath::RandRange(0.5f, 2.0f);

	FRotator NewRotation = FRotator(PitchValue, YawValue, RollValue);

	FQuat QuatRotation = FQuat(NewRotation);

	AddActorLocalRotation(QuatRotation, false, 0, ETeleportType::None);
}