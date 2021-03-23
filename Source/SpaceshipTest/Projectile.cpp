#include "Projectile.h"

#include "GameFramework/ProjectileMovementComponent.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	SM_Projectile = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_Projectile"));
	SM_Projectile->SetRelativeScale3D(FVector(0.06f, 0.06f, 0.06f));
	RootComponent = SM_Projectile;

	SM_Projectile->SetSimulatePhysics(true);
	SM_Projectile->SetEnableGravity(false);
	SM_Projectile->SetConstraintMode(EDOFMode::XZPlane);
	SM_Projectile->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SM_Projectile->SetCollisionProfileName(TEXT("PhysicsActor"));

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->InitialSpeed = 1000.0f;
	ProjectileMovement->MaxSpeed = 1000.0f;
	ProjectileMovement->Velocity = FVector(0.0f, 0.0f, 1.0f);

	Tags.AddUnique(TEXT("Projectile"));
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(10.0f);
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::SetSpeed(float Speed)
{
	ProjectileMovement->InitialSpeed = Speed;
	ProjectileMovement->MaxSpeed = Speed;
}