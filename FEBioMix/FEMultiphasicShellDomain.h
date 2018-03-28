//
//  FEMultiphasicShellDomain.hpp
//  FEBioMix
//
//  Created by Gerard Ateshian on 2/12/17.
//  Copyright © 2017 febio.org. All rights reserved.
//

#ifndef FEMultiphasicShellDomain_hpp
#define FEMultiphasicShellDomain_hpp

#include <FEBioMech/FESSIShellDomain.h>
#include "FEMultiphasic.h"
#include "FEMultiphasicDomain.h"

//-----------------------------------------------------------------------------
//! Domain class for multiphasic 3D solid elements
//! Note that this class inherits from FEElasticSolidDomain since this domain
//! also needs to calculate elastic stiffness contributions.
//!
class FEMultiphasicShellDomain : public FESSIShellDomain, public FEMultiphasicDomain
{
public:
    //! constructor
    FEMultiphasicShellDomain(FEModel* pfem);
    
    //! Reset data
    void Reset();
    
    //! get the material (overridden from FEDomain)
    FEMaterial* GetMaterial() { return m_pMat; }
    
    //! set the material
    void SetMaterial(FEMaterial* pmat);
    
    //! Unpack element data (overridden from FEDomain)
    void UnpackLM(FEElement& el, vector<int>& lm);
    
    //! initialize elements for this domain
    void PreSolveUpdate(const FETimeInfo& timeInfo);
    
    //! calculates the global stiffness matrix for this domain
    void StiffnessMatrix(FESolver* psolver, bool bsymm);
    
    //! calculates the global stiffness matrix for this domain (steady-state case)
    void StiffnessMatrixSS(FESolver* psolver, bool bsymm);
    
    //! calculates the membrane reaction stiffness matrix for this domain
    void MembraneReactionStiffnessMatrix(FESolver* psolver);
    
    //! initialize class
	bool Init() override;
    
    //! activate
    void Activate();
    
    // update domain data
    void Update(const FETimeInfo& tp);
    
    // update element state data
    void UpdateElementStress(int iel, double dt);
    
    //! Unpack element data (overridden from FEDomain)
    void UnpackMembraneLM(FEShellElement& el, vector<int>& lm);
    
public:
    
    // internal work (overridden from FEElasticDomain)
    void InternalForces(FEGlobalVector& R);
    
    // internal work (steady-state case)
    void InternalForcesSS(FEGlobalVector& R);
    
    // external work of flux generated by membrane reactions
    void MembraneReactionFluxes(FEGlobalVector& R);
    
public:
    //! element internal force vector
    void ElementInternalForce(FEShellElement& el, vector<double>& fe);
    
    //! element internal force vector (steady-state case)
    void ElementInternalForceSS(FEShellElement& el, vector<double>& fe);
    
    //! element external work of flux generated by membrane reactions
    void ElementMembraneReactionFlux(FEShellElement& el, vector<double>& fe);
    
    //! calculates the element multiphasic stiffness matrix
    bool ElementMultiphasicStiffness(FEShellElement& el, matrix& ke, bool bsymm);
    
    //! calculates the element multiphasic stiffness matrix
    bool ElementMultiphasicStiffnessSS(FEShellElement& el, matrix& ke, bool bsymm);
    
    //! calculates the element membrane flux stiffness matrix
    bool ElementMembraneFluxStiffness(FEShellElement& el, matrix& ke);
    
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

#endif /* FEMultiphasicShellDomain_hpp */
