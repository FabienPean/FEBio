#pragma once
#include "FECore/FEMaterial.h"
#include "FEBioMech/FEBodyForce.h"
#include "FEViscousFluid.h"

//-----------------------------------------------------------------------------
//! Fluid material point class.
//
class FEFluidMaterialPoint : public FEMaterialPoint
{
public:
	//! constructor
    FEFluidMaterialPoint(FEMaterialPoint* pt = 0);

	//! create a shallow copy
	FEMaterialPoint* Copy();

	//! data serialization
	void Serialize(DumpStream& ar);

	//! Data initialization
	void Init();

public:
    mat3ds RateOfDeformation() { return m_Lf.sym(); }
    mat3da Spin() { return m_Lf.skew(); }
    vec3d  Vorticity() { return vec3d(m_Lf(2,1)-m_Lf(1,2), m_Lf(0,2)-m_Lf(2,0), m_Lf(1,0)-m_Lf(0,1)); }
    
public:
    // fluid data
    vec3d       m_r0;       //!< material position
    vec3d       m_vft;      //!< fluid velocity
    vec3d       m_aft;      //!< fluid acceleration
    mat3d       m_Lf;       //!< fluid velocity gradient
    double      m_Jf;       //!< determinant of fluid deformation gradient
    double      m_Jfdot;    //!< material time derivative of Jf
    vec3d       m_gradJf;   //!< gradient of Jf
	double		m_pf;		//!< elastic fluid pressure
    mat3ds		m_sf;		//!< fluid stress
};

//-----------------------------------------------------------------------------
//! Base class for fluid materials.

class FEFluid : public FEMaterial
{
public:
	FEFluid(FEModel* pfem);
	
	// returns a pointer to a new material point object
	FEMaterialPoint* CreateMaterialPointData() override;
	
public:
	//! calculate stress at material point
	mat3ds Stress(FEMaterialPoint& pt);
	
    //! tangent of stress with respect to strain J
    mat3ds Tangent_Strain(FEMaterialPoint& mp);
    
    //! tangent of stress with respect to rate of deformation tensor D
    tens4ds Tangent_RateOfDeformation(FEMaterialPoint& mp)  { return m_pViscous->Tangent_RateOfDeformation(mp); }
    
    //! elastic pressure
    double Pressure(FEMaterialPoint& mp);
    
    //! tangent of elastic pressure with respect to strain J
    double Tangent_Pressure_Strain(FEMaterialPoint& mp) { return -m_k; }
    
    //! 2nd tangent of elastic pressure with respect to strain J
    double Tangent_Pressure_Strain_Strain(FEMaterialPoint& mp) { return 0; }
    
	//! referential fluid density
	double ReferentialDensity() { return m_rhor; }

    //! calculate current fluid density
    double Density(FEMaterialPoint& pt);
    
    //! return viscous part
    FEViscousFluid* GetViscous() { return m_pViscous; }
    
    //! kinematic viscosity
    double KinematicViscosity(FEMaterialPoint& mp);
    
    //! acoustic speed
    double AcousticSpeed(FEMaterialPoint& mp);
    
    //! bulk modulus
    double BulkModulus(FEMaterialPoint& mp);
    
    //! strain energy density
    double StrainEnergyDensity(FEMaterialPoint& mp);
    
    //! kinetic energy density
    double KineticEnergyDensity(FEMaterialPoint& mp);
    
    //! strain + kinetic energy density
    double EnergyDensity(FEMaterialPoint& mp);
    
private: // material properties
    FEPropertyT<FEViscousFluid> m_pViscous; //!< pointer to viscous part of fluid material
	
public:
    double						m_rhor;     //!< referential fluid density
    double                      m_k;        //!< bulk modulus at J=1
    
    // declare parameter list
    DECLARE_PARAMETER_LIST();
};
