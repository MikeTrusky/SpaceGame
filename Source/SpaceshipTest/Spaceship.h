#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Spaceship.generated.h"

class UFloatingPawnMovement;
class AProjectile;
class UCapsuleComponent;

UCLASS()
class SPACESHIPTEST_API ASpaceship : public APawn
{
	GENERATED_BODY()

public:
	ASpaceship();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* SM_Spaceship;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UFloatingPawnMovement* FloatingMovement;

	UPROPERTY(EditAnywhere, Category = "Shoot")
		TSubclassOf<class AProjectile> ProjectileBP;

	//Forward velocity (meters / second)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float ForwardMovementSpeed;

	//Shooting frequency (shots / second)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shoot")
		float ShootingFrequency;

	//Bullets velocity (meters / second)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shoot")
		float ProjectilesSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UCapsuleComponent* Collider;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Particles")
		class UParticleSystem* ImpactEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Particles")
		class UParticleSystem* DeathEffect;

	UPROPERTY(EditAnywhere, Category = "Audio")
		class USoundCue* ShootSound;

	UPROPERTY(EditAnywhere, Category = "Audio")
		class USoundCue* DeadSound;


protected:
	virtual void BeginPlay() override;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndexType, bool bFromSweep, const FHitResult& SweepResult);

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void MoveHorizontal(float AxisValue);

	UFUNCTION()
	void Shoot();

	UFUNCTION()
	void Dead();

	UFUNCTION()
	void EnableShooting();

	bool IsAlive = true;
	bool IsAbleToShoot = true;
};
