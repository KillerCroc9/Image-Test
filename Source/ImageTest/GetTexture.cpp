// Fill out your copyright notice in the Description page of Project Settings.


#include "GetTexture.h"
#include <ImageUtils.h>



void AGetTexture::GetPixels(UTexture2D* Texture, int32& OutWidth, int32& OutHeight)
{
    TArray<FColor> PixelArray;

    if (!Texture || !Texture->GetPlatformData())
    {
        UE_LOG(LogTemp, Error, TEXT("Texture is null or PlatformData is missing"));
        OutWidth = 0;
        OutHeight = 0;
        return ; // Return empty array to indicate an error
    }

    // Save old texture settings
    TextureCompressionSettings OldCompressionSettings = Texture->CompressionSettings;
    TextureMipGenSettings OldMipGenSettings = Texture->MipGenSettings;
    bool OldSRGB = Texture->SRGB;

    // Temporarily change texture settings for pixel reading
    Texture->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
    Texture->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
    Texture->SRGB = false;
    Texture->UpdateResource();

    FTexture2DMipMap* MipMap = &Texture->PlatformData->Mips[0];
    const FColor* FormattedImageData = static_cast<const FColor*>(MipMap->BulkData.LockReadOnly());

    OutWidth = MipMap->SizeX;
    OutHeight = MipMap->SizeY;
    PixelArray.SetNum(OutWidth * OutHeight);

    for (int32 Y = 0; Y < OutHeight; Y++)
    {
        for (int32 X = 0; X < OutWidth; X++)
        {
            FColor PixelColor = FormattedImageData[Y * OutWidth + X];
            PixelArray[Y * OutWidth + X] = PixelColor;
            SpawnCubes(PixelColor, Y, X);

        }
    }

    MipMap->BulkData.Unlock();

    //// Restore original texture settings
    
    Texture->CompressionSettings = OldCompressionSettings;
    Texture->MipGenSettings = OldMipGenSettings;
    Texture->SRGB = OldSRGB;
    Texture->UpdateResource();

}

void AGetTexture::SpawnCubes(FColor PixelColors, int32 Width, int32 Height)
{

            FVector Location(Width * 100.0f, Height * 100.0f, 0);  // Adjust the multiplier for spacing
            FRotator Rotation(0.0f, 0.0f, 0.0f);
            FActorSpawnParameters SpawnParams;
            AActor* CubeActor = GetWorld()->SpawnActor<AActor>(CubeClass, Location, Rotation, SpawnParams);
            if (CubeActor)
            {
                // Ensure there is a static mesh component to apply the material
                UStaticMeshComponent* MeshComponent = CubeActor->FindComponentByClass<UStaticMeshComponent>();
                if (MeshComponent)
                {
                    // Get the base material from the mesh component
                    UMaterialInterface* BaseMaterial = MeshComponent->GetMaterial(0); // Correctly getting material from mesh component

                    // Create a dynamic material instance from the base material
                    UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);

                    // Set the color of the dynamic material
                    DynamicMaterial->SetVectorParameterValue(FName("Base Color"), FLinearColor(PixelColors));

                    // Apply the dynamic material to the mesh component
                    MeshComponent->SetMaterial(0, DynamicMaterial);
                }
            }

    
}


