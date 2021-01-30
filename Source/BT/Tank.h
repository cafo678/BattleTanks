#pragma once

#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTankEvent);

UCLASS()
class BT_API ATank : public APawn
{
	GENERATED_BODY()

private:
	UPROPERTY (EditDefaultsOnly, Category = "Health")
	int32 StartingHealth = 100;

	UPROPERTY (VisibleAnywhere, Category = "Health")
	int32 CurrentHealth;

	virtual void BeginPlay() override;

public:
	// Sets default values for this pawn's properties
	ATank();

	// CALLED BY THE ENGINE WHEN THE ACTOR TAKE DAMAGE
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;

	UFUNCTION (BlueprintPure, Category = "Health") // It' like a const, the node in BP doesn't have a execution node (just normal node)
	float GetHealthPercent();

	//Variables for not making the tanks fire every frame
	UPROPERTY(EditAnywhere, Category = Firing)
	float ReloadTimeInSeconds = 3;

	FTankEvent OnDeath;
};
