// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#if WITH_CHAOS

#include "SimulationEditorExtender.h"
#include "Containers/BitArray.h"

class USkeletalMeshComponent;
class FPrimitiveDrawInterface;

namespace Chaos
{
	/** Chaos extension to the asset editor. */
	class FSimulationEditorExtender : public ISimulationEditorExtender
	{
	public:
		FSimulationEditorExtender();

		// ISimulationEditorExtender Interface
		virtual UClass* GetSupportedSimulationFactoryClass() override;
		virtual void ExtendViewportShowMenu(FMenuBuilder& MenuBuilder, TSharedRef<IPersonaPreviewScene> PreviewScene) override;
		virtual void DebugDrawSimulation(const IClothingSimulation* Simulation, USkeletalMeshComponent* OwnerComponent, FPrimitiveDrawInterface* PDI) override;
		// End ISimulationEditorExtender Interface

	private:
		/** Return whether or not - given the current enabled options - the simulation should be disabled. */
		bool ShouldDisableSimulation() const;
		/** Show/hide all cloth sections for the specified mesh compoment. */
		void ShowClothSections(USkeletalMeshComponent* MeshComponent, bool bIsClothSectionsVisible) const;

	private:
		/** Flags used to store the checked status for the visualization options. */
		TBitArray<> Flags;
	};
}

#endif
