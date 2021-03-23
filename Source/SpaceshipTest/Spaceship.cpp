#include "Spaceship.h"
#include "Projectile.h"

#include "Components/StaticMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystem.h"

ASpaceship::ASpaceship()
{
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	Collider->SetRelativeScale3D(FVector(2.5f, 2.5f, 2.5f));
	Collider->SetCapsuleHalfHeight(24.0f);
	Collider->SetCapsuleRadius(8.0f);
	Collider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Collider->SetCollisionProfileName(TEXT("Pawn"));
	Collider->SetSimulatePhysics(true);
	Collider->SetEnableGravity(false);
	RootComponent = Collider;

	SM_Spaceship = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM Spaceship"));
	SM_Spaceship->SetEnableGravity(false);
	SM_Spaceship->SetConstraintMode(EDOFMode::XZPlane);
	SM_Spaceship->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SM_Spaceship->SetCollisionProfileName(TEXT("CharacterMesh"));
	SM_Spaceship->SetupAttachment(RootComponent);

	FloatingMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Floating Pawn Movement"));
	FloatingMovement->MaxSpeed = 1200.0f;
	FloatingMovement->Acceleration = 4000.0f;
	
	//Initial values
	ShootingFrequency = 10.0f;
	ProjectilesSpeed = 2000.0f;
	ForwardMovementSpeed = 5.0f;

	Tags.AddUnique(TEXT("Player"));
}

void ASpaceship::BeginPlay()
{
	Super::BeginPlay();

	Collider->OnComponentBeginOverlap.AddDynamic(this, &ASpaceship::OnOverlapBegin);

	ShootingFrequency = 60.f / ShootingFrequency;

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ASpaceship::EnableShooting, ShootingFrequency / 60.f, true, 0.01f);
}

void ASpaceship::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + ForwardMovementSpeed * 100 * DeltaTime);

	SetActorLocation(NewLocation, false, nullptr, ETeleportType::None);
}

void ASpaceship::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASpaceship::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndexType, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor->ActorHasTag("Asteroid"))
	{
		IsAlive = false;

		UGameplayStatics::PlaySound2D(this, DeadSound);
	}
}

void ASpaceship::MoveHorizontal(float AxisValue)
{
	AddMovementInput(FVector(AxisValue / 2.0f, 0.0f, 0.0f), 1.0f, false);
}

void ASpaceship::Shoot()
{
	FTransform SpawnTransform = GetActorTransform();

	SpawnTransform.SetLocation(FVector(0.0f, 0.0f, 1.0f) * 100.f + GetActorLocation());

	auto MyProjectile = Cast<AProjectile>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ProjectileBP, SpawnTransform));
	if (MyProjectile != nullptr)
	{
		MyProjectile->SetSpeed(ProjectilesSpeed);

		UGameplayStatics::FinishSpawningActor(MyProjectile, SpawnTransform);
	}

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, SpawnTransform.GetLocation(), FRotator::ZeroRotator);

	IsAbleToShoot = false;

	UGameplayStatics::PlaySound2D(this, ShootSound);
}

void ASpaceship::Dead()
{
	ForwardMovementSpeed = 0.0f;

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeathEffect, this->GetActorLocation(), FRotator::ZeroRotator);

	SM_Spaceship->SetVisibility(false);
	SM_Spaceship->SetCollisionProfileName(TEXT("NoCollision"));

	Collider->SetVisibility(false);
	Collider->SetSimulatePhysics(false);
	Collider->SetCollisionProfileName(TEXT("NoCollision"));
}

void ASpaceship::EnableShooting()
{
	IsAbleToShoot = true;
}