#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Spaceship_PlayerController.generated.h"

class ACameraActor;

UCLASS()
class SPACESHIPTEST_API ASpaceship_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void SetupInputComponent() override;

	virtual void Tick(float DeltaTime) override;

	void MoveHorizontal(float AxisValue);

	UFUNCTION()
		void Shoot();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
		ACameraActor* MainCamera;

	UPROPERTY()
		float BeginDifferenceZ;
};
