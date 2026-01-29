// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Paint.h"

#include "Components/BoxComponent.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
APaint::APaint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Create BoxComponent and set as RootComponent for the Actor
    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    RootComponent = StaticMesh;
    
    BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
    BoxCollision->SetupAttachment(StaticMesh);
    
}

// Called when the game starts or when spawned
void APaint::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APaint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FString APaint::GetMainColor(){
    
    UMaterialInterface* Material = StaticMesh->GetMaterial(2);
    if (Material)
    {
        FString MaterialName = Material->GetName();

        if (MaterialName == "M_paintRed"){
            return TEXT("Red");
        } 
        else if(MaterialName == "M_paintYellow"){
            return TEXT("Yellow");
        } 
        else if(MaterialName == "M_paintBlue"){
            return TEXT("Blue");
        }
        else if(MaterialName == "M_paintGreen"){
            return TEXT("Green");
        } 
        else if(MaterialName == "M_paintOrange"){
            return TEXT("Orange");
        }
        else if(MaterialName == "M_paintPurple"){
            return TEXT("Purple");
        } 
        else{
            return TEXT("Uncolored");
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No material at the object "));
    } 
    return TEXT("Uncolored");
}
