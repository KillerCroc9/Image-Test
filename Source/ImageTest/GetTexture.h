// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GetTexture.generated.h"

UCLASS()
class IMAGETEST_API AGetTexture : public AActor
{
	GENERATED_BODY()
	
public:	

	UFUNCTION(BlueprintCallable, Category = "MaterialUtilities")
	void GetPixels(UTexture2D* Texture, int32& OutWidth, int32& OutHeight);

	void SpawnCubes(FColor PixelColors, int32 Width, int32 Height);

	UPROPERTY(EditDefaultsOnly, Category = "Cube")
	TSubclassOf<AActor> CubeClass;

};
