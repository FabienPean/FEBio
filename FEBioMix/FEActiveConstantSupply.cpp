//
//  FEActiveConstantSupply.cpp
//  FEBioMix
//
//  Created by Gerard Ateshian on 12/9/14.
//  Copyright (c) 2014 febio.org. All rights reserved.
//

#include "FEActiveConstantSupply.h"
#include "FEBioMech/FEElasticMaterial.h"

// define the material parameters
BEGIN_FECORE_CLASS(FEActiveConstantSupply, FEActiveMomentumSupply)
    ADD_PARAMETER(m_asupp, "supply");
END_FECORE_CLASS();

//-----------------------------------------------------------------------------
//! Constructor.
FEActiveConstantSupply::FEActiveConstantSupply(FEModel* pfem) : FEActiveMomentumSupply(pfem)
{
    m_asupp = 0;
}

//-----------------------------------------------------------------------------
//! Active momentum supply vector.
//! The momentum supply is oriented along the first material axis
vec3d FEActiveConstantSupply::ActiveSupply(FEMaterialPoint& mp)
{
    FEElasticMaterialPoint& et = *mp.ExtractData<FEElasticMaterialPoint>();

	// get the local coordinate systems
	mat3d Q = GetLocalCS(mp);

    // active momentum supply vector direction
    vec3d V(Q[0][0], Q[1][0], Q[2][0]);
    
    mat3d F = et.m_F;
    vec3d pw = (F*V)*m_asupp;

    return pw;
}

//-----------------------------------------------------------------------------
//! Tangent of permeability
vec3d FEActiveConstantSupply::Tangent_ActiveSupply_Strain(FEMaterialPoint &mp)
{
    return vec3d(0,0,0);
}
