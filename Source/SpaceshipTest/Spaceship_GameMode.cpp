#include "Spaceship_GameMode.h"
#include "Spaceship.h"
#include "Asteroid.h"

#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"


void ASpaceship_GameMode::BeginPlay()
{
	Super::BeginPlay();

	float TimerFrequency = (60.f / SpawningFrequency) / 60.f;
	GetWorldTimerManager().SetTimer(SpawningTimerHandle, this, &ASpaceship_GameMode::SpawnAsteroids, TimerFrequency, true, 1.0f);

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ASpaceship_GameMode::IncreaseSpawnFrequency, 1.0f, true, -1);
}

void ASpaceship_GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerSpaceship == nullptr)
		PlayerSpaceship = Cast<ASpaceship>(UGameplayStatics::GetActorOfClass(GetWorld(), ASpaceship::StaticClass()));

	if (!PlayerSpaceship->IsAlive && !Finished)
	{
		Finished = true;
		PlayerSpaceship->Dead();
	}
}

void ASpaceship_GameMode::SpawnAsteroids()
{
	FVector2D ViewportSize = FVector2D(0.0f, 0.0f);
	GEngine->GameViewport->GetViewportSize(ViewportSize);

	float x = (ViewportSize.X - 0.3f * ViewportSize.X) / 2;
	float z = PlayerSpaceship->GetActorLocation().Z + ViewportSize.Y + 0.4f * ViewportSize.Y;

	FTransform SpawnTransform;

	SpawnTransform.SetLocation(FVector(FMath::RandRange(-x, x), PlayerSpaceship->GetActorLocation().Y, z));

	float AsteroidSize = FMath::RandRange(1.5f, 4.0f);
	SpawnTransform.SetScale3D(FVector(AsteroidSize, AsteroidSize, AsteroidSize));

	FActorSpawnParameters SpawnInfo;

	GetWorld()->SpawnActor<AAsteroid>(AsteroidBP, SpawnTransform, SpawnInfo);

	float TimerFrequency = (60.f / SpawningFrequency) / 60.f;
	GetWorldTimerManager().SetTimer(SpawningTimerHandle, this, &ASpaceship_GameMode::SpawnAsteroids, TimerFrequency, true, -1);
}

void ASpaceship_GameMode::IncreaseSpawnFrequency()
{
	SpawningFrequency += SpawningFrequencyIncreaseValue;
}
