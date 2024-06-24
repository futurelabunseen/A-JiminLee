// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DecalStruct.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct FDecalStruct
{
	GENERATED_USTRUCT_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UMaterial> Material;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector Location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FRotator Rotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector Scale;

public:
	#pragma region GetSetÇÔ¼ö
		void SetMaterial(TObjectPtr<UMaterial> NewMaterial) { Material = NewMaterial; }
		TObjectPtr<UMaterial> GetMaterial() { return Material; }

		void SetLocation(FVector NewLocation) { Location = NewLocation; }
		FVector GetLocation() { return Location; }

		void SetRotation(FRotator NewRotation) { Rotation = NewRotation; }
		FRotator GetRotation() { return Rotation; }

		void SetScale(FVector NewScale) { Scale = NewScale; }
		FVector GetScale() { return Scale; }
	#pragma endregion
};
