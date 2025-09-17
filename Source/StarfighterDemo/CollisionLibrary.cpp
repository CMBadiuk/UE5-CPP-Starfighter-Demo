// CollisionLibrary.cpp

#include "CollisionLibrary.h"
#include "DrawDebugHelpers.h" // Add this include for debug drawing
#include "CollisionQueryParams.h"
#include "Kismet/KismetSystemLibrary.h"

bool UCollisionLibrary::ConeTraceMulti(
	const UObject* WorldContextObject,
	const FVector Start,
	const FRotator Direction,
	float ConeHeight,
	float ConeHalfAngle,
	ETraceTypeQuery TraceChannel,
	bool bTraceComplex,
	const TArray<AActor*>& ActorsToIgnore,
	EDrawDebugTrace::Type DrawDebugType,
	TArray<FHitResult>& OutHits,
	FLinearColor TraceColor,
	FLinearColor TraceHitColor,
	float DrawTime)
{
    // ... (Paste the entire function body from the tutorial here) ...
    
	OutHits.Reset();

	ECollisionChannel CollisionChannel = UEngineTypes::ConvertToCollisionChannel(TraceChannel);
	FCollisionQueryParams Params(SCENE_QUERY_STAT(ConeTraceMulti), bTraceComplex);
	Params.bReturnPhysicalMaterial = true;
	Params.AddIgnoredActors(ActorsToIgnore);

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
	{
		return false;
	}

	TArray<FHitResult> TempHitResults;
	const FVector End = Start + (Direction.Vector() * ConeHeight);
	const double ConeHalfAngleRad = FMath::DegreesToRadians(ConeHalfAngle);
	const double ConeBaseRadius = ConeHeight * tan(ConeHalfAngleRad);
	const FCollisionShape SphereSweep = FCollisionShape::MakeSphere(ConeBaseRadius);

	World->SweepMultiByChannel(TempHitResults, Start, End, Direction.Quaternion(), CollisionChannel, SphereSweep, Params);

	for (FHitResult& HitResult : TempHitResults)
	{
		const FVector HitDirection = (HitResult.ImpactPoint - Start).GetSafeNormal();
		const double Dot = FVector::DotProduct(Direction.Vector(), HitDirection);
		const double DeltaAngle = FMath::Acos(Dot);

		if (DeltaAngle > ConeHalfAngleRad)
		{
			continue;
		}

		const double Distance = (HitResult.ImpactPoint - Start).Length();
		const double LengthAtAngle = ConeHeight / cos(DeltaAngle);

		if (Distance > LengthAtAngle)
		{
			continue;
		}

		OutHits.Add(HitResult);
	}

#if ENABLE_DRAW_DEBUG
	if (DrawDebugType != EDrawDebugTrace::None)
	{
		const bool bPersistent = DrawDebugType == EDrawDebugTrace::Persistent;
		const float LifeTime = (DrawDebugType == EDrawDebugTrace::ForDuration) ? DrawTime : 0.f;

		// Cone trace.
		DrawDebugCone(World, Start, Direction.Vector(), ConeHeight, ConeHalfAngleRad, ConeHalfAngleRad, 12, TraceColor.ToFColor(true), bPersistent, LifeTime);

		// Successful hits.
		for (const FHitResult& Hit : OutHits)
		{
			DrawDebugPoint(World, Hit.ImpactPoint, 10, TraceHitColor.ToFColor(true), bPersistent, LifeTime);
		}
	}
#endif

	return (OutHits.Num() > 0);
}
