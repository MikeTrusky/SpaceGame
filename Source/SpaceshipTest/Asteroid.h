#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Asteroid.generated.h"

class UFloatingPawnMovement;
class AAIController;
class UCapsuleComponent;
class UParticleSystem;
class USoundCue;

UCLASS()
class SPACESHIPTEST_API AAsteroid : public APawn
{
	GENERATED_BODY()

public:
	AAsteroid();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* SM_Asteroid;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Particles")
		UParticleSystem* ImpactEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UFloatingPawnMovement* FloatingMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UCapsuleComponent* Collider;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndexType, bool bFromSweep, const FHitResult& SweepResult);

public:
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, Category = "Audio")
		USoundCue* DestroySound;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
		float PitchValue;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
		float YawValue;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
		float RollValue;

	UPROPERTY()
		AAIController* AIController = nullptr;

	UPROPERTY()
		FVector MovementDestination;
	//Variable responsible for whether the asteroid will follow the player's location or not move
	UPROPERTY()
		bool FollowPlayerLocation = false;

	UPROPERTY()
		AActor* PlayerActor = nullptr;

	UFUNCTION()
		void FindPlayer();

	UFUNCTION()
		void MoveTowardPlayer();

	UFUNCTION()
		void RotateAsteroid();
};
