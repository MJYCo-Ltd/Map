#include <cmath>
#include "Sofa/sofam.h"
#include "StarToneReproducer.h"

#ifndef pow10
#define pow10(x) exp((x) * 2.3025850930)
#endif

CStarToneReproducer::CStarToneReproducer() :
    Lda(50.f), Lwa(40000.f), oneOverMaxdL(1.f / 100.f), lnOneOverMaxdL(
                                                            std::log(1.f / 100.f)), oneOverGamma(1.f / 2.2222f)
{
    // Initialize  sensor
    setInputScale();

    // Update alphaDa and betaDa values
    float log10Lwa = std::log10(Lwa);
    alphaWa = 0.4f * log10Lwa + 1.619f;
    betaWa = -0.4f * log10Lwa * log10Lwa + 0.218f * log10Lwa + 6.1642f;

    setDisplayAdaptationLuminance(Lda);
    setWorldAdaptationLuminance(Lwa);
}
CStarToneReproducer::~CStarToneReproducer()
{
}

void CStarToneReproducer::setDisplayAdaptationLuminance(float displayAdaptationLuminance)
{
    Lda = displayAdaptationLuminance;

    // Update alphaDa and betaDa values
    float log10Lda = std::log10(Lda);
    alphaDa = 0.4f * log10Lda + 1.619f;
    betaDa = -0.4f * log10Lda * log10Lda + 0.218f * log10Lda + 6.1642f;

    // Update terms
    alphaWaOverAlphaDa = alphaWa / alphaDa;
    term2 = pow10((betaWa-betaDa)/alphaDa) / (DPI * 0.0001f);
    lnTerm2 = std::log(term2);
    term2TimesOneOverMaxdLpOneOverGamma = std::pow(term2 * oneOverMaxdL,
                                                   oneOverGamma);
}

void CStarToneReproducer::setWorldAdaptationLuminance(float worldAdaptationLuminance)
{
    Lwa = worldAdaptationLuminance;

    // Update alphaDa and betaDa values
    float log10Lwa = std::log10(Lwa);
    alphaWa = 0.4f * log10Lwa + 1.619f;
    betaWa = -0.4f * log10Lwa * log10Lwa + 0.218f * log10Lwa + 6.1642f;

    // Update terms
    alphaWaOverAlphaDa = alphaWa / alphaDa;
    term2 = pow10((betaWa-betaDa)/alphaDa) / (DPI * 0.0001f);
    lnTerm2 = std::log(term2);
    term2TimesOneOverMaxdLpOneOverGamma = std::pow(term2 * oneOverMaxdL,
                                                   oneOverGamma);
}

float CStarToneReproducer::getWorldAdaptationLuminance() const
{
    return Lwa;
}

void CStarToneReproducer::setInputScale(float scale)
{
    inputScale = scale;
    lnInputScale = std::log(inputScale);
}

float CStarToneReproducer::getInputScale() const
{
    return inputScale;
}

void CStarToneReproducer::setMaxDisplayLuminance(float maxdL)
{
    oneOverMaxdL = 1.f / maxdL;
    lnOneOverMaxdL = std::log(oneOverMaxdL);
    term2TimesOneOverMaxdLpOneOverGamma = std::pow(term2 * oneOverMaxdL,
                                                   oneOverGamma);
}

float CStarToneReproducer::getDisplayGamma() const
{
    return 1.f / oneOverGamma;
}

void CStarToneReproducer::setDisplayGamma(float gamma)
{
    oneOverGamma = 1.f / gamma;
    term2TimesOneOverMaxdLpOneOverGamma = std::pow(term2 * oneOverMaxdL,
                                                   oneOverGamma);
}

float CStarToneReproducer::adaptLuminance(float worldLuminance) const
{
    return std::pow(
                (float) (inputScale * worldLuminance * DPI * 0.0001f),
                alphaWaOverAlphaDa) * term2;
}

float CStarToneReproducer::reverseAdaptLuminance(float displayLuminance) const
{
    return std::pow((float) (displayLuminance / term2),
                    1.f / alphaWaOverAlphaDa) / (inputScale * DPI * 0.0001f);
}

float CStarToneReproducer::adaptLuminanceScaled(float worldLuminance) const
{
    return adaptLuminance(worldLuminance) * oneOverMaxdL;
}

float CStarToneReproducer::reverseAdaptLuminanceScaled(float displayLuminance) const
{
    return reverseAdaptLuminance(displayLuminance / oneOverMaxdL);
}

float CStarToneReproducer::adaptLuminanceScaledLn(float lnWorldLuminance,float pFact) const
{
    static const float lnPix0p0001 = -8.0656104861f;
    return std::exp(
                ((lnInputScale + lnWorldLuminance + lnPix0p0001)
                 * alphaWaOverAlphaDa + lnTerm2 + lnOneOverMaxdL)
                * pFact);
}

void CStarToneReproducer::getShadersParams(float& a, float& b, float & c) const
{
    a = alphaWaOverAlphaDa;
    b = oneOverGamma;
    c = term2TimesOneOverMaxdLpOneOverGamma;
}

