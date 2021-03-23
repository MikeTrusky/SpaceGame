#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Spaceship_GameMode.generated.h"

class AAsteroid;
class ASpaceship;

UCLASS()
class SPACESHIPTEST_API ASpaceship_GameMode : public AGameMode
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Asteroids")
		TSubclassOf<AAsteroid> AsteroidBP;

	//Initial appearance frequency (number / second)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asteroids")
		float SpawningFrequency;

	//Appearance frequency increase (number / second)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asteroids")
		float SpawningFrequencyIncreaseValue;

	UPROPERTY(BlueprintReadWrite)
		bool Finished = false;

	UFUNCTION()
		void SpawnAsteroids();

	UFUNCTION()
		void IncreaseSpawnFrequency();

private:
	FTimerHandle SpawningTimerHandle;

	UPROPERTY()
		ASpaceship* PlayerSpaceship = nullptr;

};
