// Fill out your copyright notice in the Description page of Project Settings.


#include "IBMachineAction.h"
#include "Crafting/IBCraftingMachine.h"

void UIBMachineAction::PerformAction_Implementation(AIBCraftingMachine* CraftingMachine)
{
	CraftingMachineRef = CraftingMachine;
}
