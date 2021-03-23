#include "Spaceship_PlayerController.h"
#include "Spaceship.h"

#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"


void ASpaceship_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	EnableInput(this);

	InputComponent->BindAxis("MoveHorizontal", this, &ASpaceship_PlayerController::MoveHorizontal);
	InputComponent->BindAction("Shoot", IE_Pressed, this, &ASpaceship_PlayerController::Shoot);
}

void ASpaceship_PlayerController::BeginPlay()
{
	TArray<AActor*> CameraActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), CameraActors);

	FViewTargetTransitionParams Params;
	SetViewTarget(CameraActors[0], Params);

	MainCamera = Cast<ACameraActor>(CameraActors[0]);

	auto MyPawn = Cast<ASpaceship>(GetPawn());

	if (MyPawn)
	{
		if (MainCamera)
			BeginDifferenceZ = MainCamera->GetActorLocation().Z - MyPawn->GetActorLocation().Z;
	}
}

void ASpaceship_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MainCamera != nullptr)
	{
		auto MyPawn = Cast<ASpaceship>(GetPawn());

		if (MyPawn)
		{
			MainCamera->SetActorLocation(FVector(MainCamera->GetActorLocation().X, MainCamera->GetActorLocation().Y, MyPawn->GetActorLocation().Z + BeginDifferenceZ));
		}
	}
}

void ASpaceship_PlayerController::MoveHorizontal(float AxisValue)
{
	auto MyPawn = Cast<ASpaceship>(GetPawn());

	if (MyPawn)
	{
		MyPawn->MoveHorizontal(AxisValue);
	}
}

void ASpaceship_PlayerController::Shoot()
{
	auto MyPawn = Cast<ASpaceship>(GetPawn());

	if (MyPawn && MyPawn->IsAbleToShoot)
	{
		MyPawn->Shoot();
	}
}
