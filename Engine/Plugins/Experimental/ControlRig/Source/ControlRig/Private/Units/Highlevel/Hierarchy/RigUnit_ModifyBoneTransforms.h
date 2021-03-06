// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Units/RigUnit.h"
#include "Units/Highlevel/RigUnit_HighlevelBase.h"
#include "RigUnit_ModifyBoneTransforms.generated.h"

UENUM()
enum class EControlRigModifyBoneMode : uint8
{
	/** Override existing local transform */
	OverrideLocal,

	/** Override existing global transform */
	OverrideGlobal,

	/** 
	 * Additive to existing local transform.
	 * Input transform is added within the bone's space.
	 */
	AdditiveLocal,

	/**
     * Additive to existing global transform.
     * Input transform is added as a global offset in the root of the hierarchy.
	 */
	AdditiveGlobal,

	/** MAX - invalid */
	Max UMETA(Hidden),
};

USTRUCT()
struct FRigUnit_ModifyBoneTransforms_PerBone
{
	GENERATED_BODY()

	FRigUnit_ModifyBoneTransforms_PerBone()
		: Bone(NAME_None)
	{
	}

	/**
	 * The name of the Bone to set the transform for.
	 */
	UPROPERTY(EditAnywhere, meta = (Input, CustomWidget = "BoneName", Constant), Category = FRigUnit_ModifyBoneTransforms_PerBone)
	FName Bone;

	/**
	 * The transform value to set for the given Bone.
	 */
	UPROPERTY(EditAnywhere, meta = (Input), Category = FRigUnit_ModifyBoneTransforms_PerBone)
	FTransform Transform;
};

USTRUCT()
struct FRigUnit_ModifyBoneTransforms_WorkData
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<int32> CachedBoneIndices;
};

/**
 * ModifyBonetransforms is used to perform a change in the hierarchy by setting one or more bones' transforms.
 */
USTRUCT(meta=(DisplayName="Modify Transforms", Category="Hierarchy", DocumentationPolicy="Strict", Keywords = "ModifyBone"))
struct FRigUnit_ModifyBoneTransforms : public FRigUnit_HighlevelBaseMutable
{
	GENERATED_BODY()

	FRigUnit_ModifyBoneTransforms()
		: Weight(1.f)
		, WeightMinimum(0.f)
		, WeightMaximum(1.f)
		, Mode(EControlRigModifyBoneMode::AdditiveLocal)
	{
		BoneToModify.Add(FRigUnit_ModifyBoneTransforms_PerBone());
	}

	virtual FName DetermineSpaceForPin(const FString& InPinPath, void* InUserContext) const override
	{
		if (InPinPath.StartsWith(TEXT("BoneToModify")))
		{
			int32 Index = INDEX_NONE;
			FString Left, Middle, Right;
			if (InPinPath.Replace(TEXT("["), TEXT(".")).Split(TEXT("."), &Left, &Middle))
			{
				if (Middle.Replace(TEXT("]"), TEXT(".")).Split(TEXT("."), &Left, &Right))
				{
					Index = FCString::Atoi(*Left);
				}
			}

			if (BoneToModify.IsValidIndex(Index))
			{
				if (Mode == EControlRigModifyBoneMode::AdditiveLocal)
				{
					return BoneToModify[Index].Bone;
				}

				if (Mode == EControlRigModifyBoneMode::OverrideLocal)
				{
					if (const FRigHierarchyContainer* Container = (const FRigHierarchyContainer*)InUserContext)
					{
						int32 BoneIndex = Container->BoneHierarchy.GetIndex(BoneToModify[Index].Bone);
						if (BoneIndex != INDEX_NONE)
						{
							return Container->BoneHierarchy[BoneIndex].ParentName;
						}
					}
				}
			}
		}
		return NAME_None;
	}

	RIGVM_METHOD()
	virtual void Execute(const FRigUnitContext& Context) override;

	/**
	 * The bones to modify.
	 */
	UPROPERTY(meta = (Input, ExpandByDefault, DefaultArraySize=1))
	TArray<FRigUnit_ModifyBoneTransforms_PerBone> BoneToModify;

	/**
	 * At 1 this sets the transform, between 0 and 1 the transform is blended with previous results.
	 */
	UPROPERTY(meta = (Input, ClampMin=0.f, ClampMax=1.f, UIMin = 0.f, UIMax = 1.f))
	float Weight;

	/**
	 * The minimum of the weight - defaults to 0.0
	 */
	UPROPERTY(meta = (Input, Constant, ClampMin = 0.f, ClampMax = 1.f, UIMin = 0.f, UIMax = 1.f))
	float WeightMinimum;

	/**
	 * The maximum of the weight - defaults to 1.0
	 */
	UPROPERTY(meta = (Input, Constant, ClampMin = 0.f, ClampMax = 1.f, UIMin = 0.f, UIMax = 1.f))
	float WeightMaximum;

	/**
	 * Defines if the bone's transform should be set
	 * in local or global space, additive or override.
	 */
	UPROPERTY(meta = (Input, Constant))
	EControlRigModifyBoneMode Mode;

	// Used to cache the internally used bone index
	UPROPERTY(transient)
	FRigUnit_ModifyBoneTransforms_WorkData WorkData;
};
