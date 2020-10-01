// Fill out your copyright notice in the Description page of Project Settings.


#include "MyLocalPlayer.h"
#include "Engine.h"

static double _shiftLensValue = 0.0;
static bool flag = false;

static FString FloatToString(float Value)
{
	if (FMath::IsNaN(Value))
	{
		return TEXT("0.0f");
	}
	return FString::Printf(TEXT("%f"), Value);
}
void UMyLocalPlayer::PostInitProperties()
{
	Super::PostInitProperties();
	_shiftLensValue = 0.0;
}

FSceneView * UMyLocalPlayer::CalcSceneView(FSceneViewFamily * ViewFamily, FVector &OutViewLocation, FRotator &OutViewRotation, FViewport * Viewport, FViewElementDrawer * ViewDrawer, EStereoscopicPass StereoPass)
{
	FSceneView* View = ULocalPlayer::CalcSceneView(ViewFamily, OutViewLocation, OutViewRotation, Viewport, ViewDrawer, StereoPass);
	if (View) {
		FMatrix CurrentMatrix = View->ViewMatrices.GetProjectionMatrix();
/*
		//float FOV = FMath::DegreesToRadians(30.0f);
		//FMatrix ProjectionMatrix = FReversedZPerspectiveMatrix(FOV, 16.0f, 9.0f, GNearClippingPlane);

		FPlane FarPlane(View->ViewMatrices.GetViewOrigin() + View->GetViewDirection() * View->SceneViewInitOptions.OverrideFarClippingPlaneDistance, View->GetViewDirection());

		double aspect = 16.0 / 9.0;
		double Width = 1.0;
		double Height = 1.0/aspect;

		double Top    =  Height / 2.0;
		double Bottom = -Height / 2.0;
		double Left   = -Width  / 2.0;
		double Right  =  Width  / 2.0;
		double ZNear  =  GNearClippingPlane;
		double ZFar   =  1000;

		double A =  (Right + Left) / (Right - Left);
		double B =  (Top + Bottom) / (Top - Bottom);
		double C = -(ZFar + ZNear) / (ZFar - ZNear);
		double D = -2.0 * ZFar * ZNear / (ZFar - ZNear);

		FMatrix newMatrix = FMatrix(
			FPlane(2.0*ZNear/(Right-Left),	0,						0,	0),
			FPlane(0,						2.0*ZNear/(Top-Bottom),	0,	0),
			FPlane(A,						B,						C,	1),
			FPlane(0,						0,						D,	0)
		);

		//float fovrad = //FMath::Atan(1.f / CurrentMatrix.M[0][0]);
		//float fovdeg = View->FOV;// SceneViewInitOptions.FOV;// FMath::RadiansToDegrees(fovrad);
		//FIntRect Rect = View->SceneViewInitOptions.GetViewRect();
*/
		float Fov = View->FOV;// 90.f;
		Fov = PlayerController->PlayerCameraManager->GetFOVAngle(); // �ǂ�������H
		float HalfFov = FMath::DegreesToRadians(Fov) / 2.f;
		float Width = ViewportClient->Viewport->GetSizeXY().X;	// Rect.Width();
		float Height = ViewportClient->Viewport->GetSizeXY().Y;	// Rect.Height();
		float ZNear = GNearClippingPlane;

		FMatrix newMatrix = FMatrix(
			FPlane(1.0 / FMath::Tan(HalfFov), 0, 0, 0),
			FPlane(0, Width / Height / FMath::Tan(HalfFov), 0, 0),
			FPlane(0, _shiftLensValue, 0, 1),		// shift lens value.
			FPlane(0, 0, ZNear, 0)
		);


		if (_shiftLensValue > 0) {
			flag = false;
		}
		else if (_shiftLensValue < -1) {
			flag = true;
		}
		
		if (flag) {
			_shiftLensValue += 0.002;
		}
		else {
			_shiftLensValue -= 0.002;
		}

		View->UpdateProjectionMatrix(newMatrix);



		FString Message = newMatrix.ToString();
		//Message = FloatToString(View->FOV);
		Message = FString("�����苭����") + FloatToString(_shiftLensValue);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, Message);

	}

	return View;
}