#include "HexMapPrivatePCH.h"
#include "HexMap.h"
#include "HexCoord.h"

const std::vector<FHexCoord> FHexCoord::HexDirections = { FHexCoord::Init(1, 0, -1), FHexCoord::Init(1, -1, 0), FHexCoord::Init(0, -1, 1), FHexCoord::Init(-1, 0, 1), FHexCoord::Init(-1, 1, 0), FHexCoord::Init(0, 1, -1) };

FHexCoord FHexCoord::Init(int32 Q_, int32 R_, int32 S_)
{
	FHexCoord HexCoord;
	HexCoord.Q = Q_;
	HexCoord.R = R_;
	HexCoord.S = S_;
	return HexCoord;
}

FHexCoord FHexCoord::Round(const FFractionalHexCoord& FractionalHexCoord)
{
	FHexCoord HexCoord;
	HexCoord.Q = int32(round(FractionalHexCoord.Q));
	HexCoord.R = int32(round(FractionalHexCoord.R));
	HexCoord.S = int32(round(FractionalHexCoord.S));
	int32 Qdiff = std::abs(HexCoord.Q - FractionalHexCoord.Q);
	int32 Rdiff = std::abs(HexCoord.R - FractionalHexCoord.R);
	int32 Sdiff = std::abs(HexCoord.S - FractionalHexCoord.S);
	if (Qdiff > Rdiff && Qdiff > Sdiff)
	{
		HexCoord.Q = -HexCoord.R - HexCoord.S;
	}
	else
	{
		if (Rdiff > Sdiff)
		{
			HexCoord.R = -HexCoord.Q - HexCoord.S;
		}
		else
		{
			HexCoord.S = -HexCoord.Q - HexCoord.R;
		}
	}
	return HexCoord;
}

bool FHexCoord::Compare(const FHexCoord& HexCoord1, const FHexCoord& HexCoord2)
{
	return HexCoord1.Q == HexCoord2.Q && HexCoord1.R == HexCoord2.R && HexCoord1.S == HexCoord2.S;
}

FVector2D FHexCoord::QOffsetFromCube(EHexDirection Direction, const FHexCoord& HexCoord)
{
	int32 Col = HexCoord.Q;
	int32 Row = HexCoord.R + int32((HexCoord.Q + (int32)Direction * (HexCoord.Q & 1)) / 2);
	return FVector2D(Col, Row);
}

FHexCoord FHexCoord::QOffsetToCube(EHexDirection Direction, const FVector2D& Location)
{
	int32 Q = Location.X;
	int32 R = Location.Y - int32((Location.X + (int32)Direction * ((int32)Location.X & 1)) / 2);
	int32 S = -Q - R;
	return FHexCoord::Init(Q, R, S);
}

FVector2D FHexCoord::ROffsetFromCube(EHexDirection Direction, const FHexCoord& HexCoord)
{
	int32 Col = HexCoord.Q + int32((HexCoord.R + (int32)Direction * (HexCoord.R & 1)) / 2);
	int32 Row = HexCoord.R;
	return FVector2D(Col, Row);
}

FHexCoord FHexCoord::ROffsetToCube(EHexDirection Direction, const FVector2D& Location)
{
	int32 Q = Location.X - int32((Location.Y + (int32)Direction * ((int32)Location.Y & 1)) / 2);
	int32 R = Location.Y;
	int32 S = -Q - R;
	return FHexCoord::Init(Q, R, S);
}

FVector2D FHexCoord::ToLocation(const FHexLayout& Layout, const FHexCoord& HexCoord)
{
	float X = (Layout.Orientation.F0 * HexCoord.Q + Layout.Orientation.F1 * HexCoord.R) * Layout.Size.X;
	float Y = (Layout.Orientation.F2 * HexCoord.Q + Layout.Orientation.F3 * HexCoord.R) * Layout.Size.Y;
	return FVector2D(X + Layout.Origin.X, Y + Layout.Origin.Y);
}

FFractionalHexCoord FHexCoord::ToHex(const FHexLayout& Layout, const FVector2D& Location)
{
	FVector2D Point = FVector2D((Location.X - Layout.Origin.X) / Layout.Size.X, (Location.Y - Layout.Origin.Y) / Layout.Size.Y);
	float Q = Layout.Orientation.B0 * Point.X + Layout.Orientation.B1 * Point.Y;
	float R = Layout.Orientation.B2 * Point.X + Layout.Orientation.B3 * Point.Y;
	return FFractionalHexCoord::Init(Q, R, -Q - R);
}

FIntVector FHexCoord::ToVec() const
{
	return FIntVector(Q, R, S);
}

FFractionalHexCoord FFractionalHexCoord::Init(float Q_, float R_, float S_)
{
	FFractionalHexCoord FractionalHexCoord;
	FractionalHexCoord.Q = Q_;
	FractionalHexCoord.R = R_;
	FractionalHexCoord.S = S_;
	return FractionalHexCoord;
}

FHexOrientation FHexOrientation::Init(float F0_, float F1_, float F2_, float F3_, float B0_, float B1_, float B2_, float B3_, float StartAngle_)
{
	FHexOrientation HexOrientation;
	HexOrientation.F0 = F0_;
	HexOrientation.F1 = F1_;
	HexOrientation.F2 = F2_;
	HexOrientation.F3 = F3_;
	HexOrientation.B0 = B0_;
	HexOrientation.B1 = B1_;
	HexOrientation.B2 = B2_;
	HexOrientation.B3 = B3_;
	HexOrientation.StartAngle = StartAngle_;
	return HexOrientation;
}

const FHexOrientation FHexLayout::Pointy = FHexOrientation::Init(sqrt(3.0), sqrt(3.0) / 2.0, 0.0, 3.0 / 2.0, sqrt(3.0) / 3.0, -1.0 / 3.0, 0.0, 2.0 / 3.0, 0.5);
const FHexOrientation FHexLayout::Flat = FHexOrientation::Init(3.0 / 2.0, 0.0, sqrt(3.0) / 2.0, sqrt(3.0), 2.0 / 3.0, 0.0, -1.0 / 3.0, sqrt(3.0) / 3.0, 0.0);

FHexLayout FHexLayout::Init(const FHexOrientation& Orientation_, const FVector2D& Size_, const FVector2D& Origin_)
{
	FHexLayout HexLayout;
	HexLayout.Orientation = Orientation_;
	HexLayout.Size = Size_;
	HexLayout.Origin = Origin_;
	return HexLayout;
}