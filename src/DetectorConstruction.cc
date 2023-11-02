#include "DetectorConstruction.hh"

#include "G4AutoDelete.hh"
#include "G4Box.hh"
#include "G4Colour.hh"
#include "G4Element.hh"
#include "G4ExtrudedSolid.hh"
#include "G4GeometryManager.hh"
#include "G4GeometryTolerance.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4Isotope.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4OpticalMaterialProperties.hh"
#include "G4OpticalSurface.hh"
#include "G4PhysicalConstants.hh"
#include "G4PVPlacement.hh"
#include "G4QuadrangularFacet.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4Sphere.hh"
#include "G4SubtractionSolid.hh"
#include "G4SystemOfUnits.hh"
#include "G4TessellatedSolid.hh"
#include "G4ThreeVector.hh"
#include "G4Trd.hh"
#include "G4TriangularFacet.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4UserLimits.hh"
#include "G4VisAttributes.hh"
#include "IBDHit.hh"
#include "IBDSD.hh"

#include <algorithm>
#include <cmath>
#include <numbers>
#include <utility>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
DetectorConstruction::DetectorConstruction() {
    cellNumber = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct() {

    // Define Material

    const auto nistManager = G4NistManager::Instance();
    const auto galactic = new G4Material("galactic", 1, 1.008 * g / mole, 1.e-25 * g / cm3, kStateGas, 2.73 * kelvin, 3.e-18 * pascal);

    const auto carbonElement = nistManager->FindOrBuildElement("C");
    const auto hydrogenElement = nistManager->FindOrBuildElement("H");
    const auto oxygenElement = nistManager->FindOrBuildElement("O");
    const auto nitrogenElement = nistManager->FindOrBuildElement("N");
    const auto sulfurElement = nistManager->FindOrBuildElement("S");

    const auto glass = new G4Material("glass", 1.18 * g / cm3, 3, kStateSolid);
    glass->AddElement(carbonElement, 0.59984);
    glass->AddElement(hydrogenElement, 0.08055);
    glass->AddElement(oxygenElement, 0.31961);

    const auto liquid = new G4Material("liquid", 0.859 * g / cm3, 5, kStateLiquid);
    liquid->AddElement(carbonElement, 87.924 * perCent);
    liquid->AddElement(hydrogenElement, 12.01 * perCent);
    liquid->AddElement(oxygenElement, 0.034 * perCent);
    liquid->AddElement(nitrogenElement, 0.027 * perCent);
    liquid->AddElement(sulfurElement, 0.005 * perCent);

    // Define World Volume

    const auto worldSize = 40 * m;
    const auto worldS = new G4Box("world", 0.5 * worldSize, 0.5 * worldSize, 0.5 * worldSize);
    const auto worldLV = new G4LogicalVolume(worldS, galactic, "World");
    const auto worldPV = new G4PVPlacement(nullptr, {}, worldLV, "World", nullptr, false, 0, true);

    const auto inner = 37 * m;
    const auto thickness = 20 * cm;

    const auto glassS = new G4Sphere("glass", 0.5 * inner, 0.5 * inner + thickness, 0, 2 * pi, 0, 2 * pi);
    const auto glassLV = new G4LogicalVolume(glassS, glass, "glass");
    new G4PVPlacement(nullptr, {}, glassLV, "glass", worldLV, false, 0, true);

    const auto liquidS = new G4Sphere("liquid", 0, 0.5 * inner, 0, 2 * pi, 0, 2 * pi);
    const auto liquidLV = new G4LogicalVolume(liquidS, liquid, "liquid");
    new G4PVPlacement(nullptr, {}, liquidLV, "liquid", worldLV, false, 0, true);

    ++cellNumber;

    return worldPV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructSDandField() {
    // Sensitive detectors
    auto ibdSD = new IBDSD("IBDSD", "IBDHitsCollection");
    G4SDManager::GetSDMpointer()->AddNewDetector(ibdSD);
    SetSensitiveDetector("liquid", ibdSD, true);
}