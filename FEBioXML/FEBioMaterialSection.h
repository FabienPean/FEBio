#pragma once
#include "FEBioImport.h"
#include "FEBioLib/FETransverselyIsotropic.h"
#include <FECore/FERigid.h>
#include "FEBioLib/FEElasticMixture.h"
#include "FEBioLib/FEUncoupledElasticMixture.h"
#include "FEBioLib/FEBiphasic.h"
#include "FEBioLib/FEBiphasicSolute.h"
#include "FEBioLib/FETriphasic.h"
#include "FEBioLib/FEMultiphasic.h"
#include "FEBioLib/FEViscoElasticMaterial.h"
#include "FEBioLib/FEUncoupledViscoElasticMaterial.h"
#include "FEBioLib/FEElasticMultigeneration.h"
#include "FEBioLib/FERemodelingElasticMaterial.h"

//-----------------------------------------------------------------------------
// Material Section
class FEBioMaterialSection : public FEBioFileSection
{
public:
	FEBioMaterialSection(FEFEBioImport* pim) : FEBioFileSection(pim){}
	void Parse(XMLTag& tag);

protected:
	void ParseMaterial					   (XMLTag& tag, FEMaterial* pm);
	bool ParseElasticMaterial			   (XMLTag& tag, FEElasticMaterial* pm);
	bool ParseTransIsoMaterial			   (XMLTag& tag, FETransverselyIsotropic* pm);
	bool ParseRigidMaterial				   (XMLTag& tag, FERigidMaterial* pm);
	bool ParseElasticMixture			   (XMLTag& tag, FEElasticMixture* pm);
	bool ParseUncoupledElasticMixture	   (XMLTag& tag, FEUncoupledElasticMixture* pm);
	bool ParseBiphasicMaterial		  	   (XMLTag& tag, FEBiphasic* pm);
	bool ParseBiphasicSoluteMaterial  	   (XMLTag& tag, FEBiphasicSolute* pm);
	bool ParseSoluteMaterial			   (XMLTag& tag, FESolute* pm);
	bool ParseReactionMaterial			   (XMLTag& tag, FEChemicalReaction* pm);
	bool ParseTriphasicMaterial  		   (XMLTag& tag, FETriphasic* pm);
	bool ParseMultiphasicMaterial          (XMLTag& tag, FEMultiphasic* pm);
	bool ParseViscoElasticMaterial		   (XMLTag& tag, FEViscoElasticMaterial* pm);
	bool ParseUncoupledViscoElasticMaterial(XMLTag& tag, FEUncoupledViscoElasticMaterial* pm);
	bool ParseElasticMultigeneration	   (XMLTag &tag, FEElasticMultigeneration *pm);
	bool ParseRemodelingSolid			   (XMLTag &tag, FERemodelingElasticMaterial *pm);

protected:
	int	m_nmat;
};
