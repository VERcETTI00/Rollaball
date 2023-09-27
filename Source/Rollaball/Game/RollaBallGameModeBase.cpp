// Fill out your copyright notice in the Description page of Project Settings.


#include "RollaBallGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "RollaBallWidget.h"
#include "Rollaball/Items/RollaBallItemBase.h"
#include "Blueprint/UserWidget.h"

void ARollaBallGameModeBase::BeginPlay()
{
	TArray<AActor*> Items;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARollaBallItemBase::StaticClass(), Items);
	ItemsInLevel = Items.Num();

	if (GameWidgetClass)
	{
		GameWidget = Cast<URollaBallWidget>(CreateWidget(GetWorld(), GameWidgetClass));

		if (GameWidget)
		{
			GameWidget->AddToViewport();
			UpdateItemText();
		}
	}
}

void ARollaBallGameModeBase::UpdateItemText()
{
	GameWidget->SetItemText(ItemsCollected, ItemsInLevel);
}

void ARollaBallGameModeBase::ItemCollected()
{
	ItemsCollected++;
	UpdateItemText();
}