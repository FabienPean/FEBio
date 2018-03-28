//
//  FEBiphasicSoluteShellDomain.hpp
//  FEBioMix
//
//  Created by Gerard Ateshian on 12/16/16.
//  Copyright © 2016 febio.org. All rights reserved.
//

#ifndef FEBiphasicSoluteShellDomain_hpp
#define FEBiphasicSoluteShellDomain_hpp

#include <FEBioMech/FESSIShellDomain.h>
#include "FEBiphasicSolute.h"
#include "FEBiphasicSoluteDomain.h"

//-----------------------------------------------------------------------------
//! Domain class for biphasic-solute 3D solid elements
//! Note that this class inherits from FEElasticSolidDomain since this domain
//! also needs to calculate elastic stiffness contributions.
//!
class FEBiphasicSoluteShellDomain : public FESSIShellDomain, public FEBiphasicSoluteDomain
{
public:
    //! constructor
    FEBiphasicSoluteShellDomain(FEModel* pfem);
    
    //! reset domain data
    void Reset();
    
    //! get the material (overridden from FEDomain)
    FEMaterial* GetMaterial() { return m_pMat; }
    
    //! set the material
    void SetMaterial(FEMaterial* pmat);
    
    //! Unpack solid element data (overridden from FEDomain)
    void UnpackLM(FEElement& el, vector<int>& lm);
    
    //! initialize class
	bool Init() override;
    
    //! Activate
    void Activate();
    
    //! initialize elements for this domain
    void PreSolveUpdate(const FETimeInfo& timeInfo);
    
    // update domain data
    void Update(const FETimeInfo& tp);
    
    // update element stress
    void UpdateElementStress(int iel);
    
public:
    // internal work (overridden from FEElasticDomain)
    void InternalForces(FEGlobalVector& R);
    
    // internal work (steady-state analyses)
    void InternalForcesSS(FEGlobalVector& R);
    
public:
    //! calculates the global stiffness matrix for this domain
    void StiffnessMatrix(FESolver* psolver, bool bsymm);
    
    //! calculates the global stiffness matrix for this domain (steady-state case)
    void StiffnessMatrixSS(FESolver* psolver, bool bsymm);
    
protected:
    //! element internal force vector
    void ElementInternalForce(FEShellElement& el, vector<double>& fe);
    
    //! element internal force vector (steady-state analyses)
    void ElementInternalForceSS(FEShellElement& el, vector<double>& fe);
    
    //! calculates the element solute-poroelastic stiffness matrix
    bool ElementBiphasicSoluteStiffness(FEShellElement& el, matrix& ke, bool bsymm);
    
    //! calculates the element solute-poroelastic stiffness matrix
    bool ElementBiphasicSoluteStiffnessSS(FEShellElement& el, matrix& ke, bool bsymm);
    
protected: // overridden from FEElasticDomain, but not implemented in this domain
    void BodyForce(FEGlobalVector& R, FEBodyForce& bf) {}
    void InertialForces(FEGlobalVector& R, vector<double>& F) {}
    void StiffnessMatrix(FESolver* psolver) {}
    void BodyForceStiffness(FESolver* psolver, FEBodyForce& bf) {}
    void MassMatrix(FESolver* psolver, double scale) {}
    
protected:
    int					m_dofSX;
    int					m_dofSY;
    int					m_dofSZ;
};

#endif /* FEBiphasicSoluteShellDomain_hpp */
