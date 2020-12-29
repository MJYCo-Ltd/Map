#ifndef STARTONEREPRODUCER_H
#define STARTONEREPRODUCER_H


class CStarToneReproducer
{
public:
    CStarToneReproducer();
    virtual ~CStarToneReproducer();

    void setDisplayAdaptationLuminance(float displayAdaptationLuminance);

    void setWorldAdaptationLuminance(float worldAdaptationLuminance);
    float getWorldAdaptationLuminance() const;

    void setInputScale(float scale = 1.f);
    float getInputScale() const;

    void setMaxDisplayLuminance(float maxdL);

    float adaptLuminance(float worldLuminance) const;
    float reverseAdaptLuminance(float displayLuminance) const;

    void setDisplayGamma(float gamma);
    float getDisplayGamma() const;

    float adaptLuminanceScaled(float worldLuminance) const;
    float reverseAdaptLuminanceScaled(float displayLuminance) const;

    float adaptLuminanceScaledLn(float lnWorldLuminance,float pFact = 0.5f) const;

    void getShadersParams(float& a, float& b, float & c) const;
public:
    // The global luminance scaling
    float inputScale;
    float lnInputScale;		// std::log(inputScale)

    float Lda;		// Display luminance adaptation (in cd/m^2)
    float Lwa;		// World   luminance adaptation (in cd/m^2)
    float oneOverMaxdL;	// 1 / Display maximum luminance (in cd/m^2)
    float lnOneOverMaxdL; // log(oneOverMaxdL)
    float oneOverGamma;	// 1 / Screen gamma value

    // Precomputed variables
    float alphaDa;
    float betaDa;
    float alphaWa;
    float betaWa;
    float alphaWaOverAlphaDa;
    float term2;
    float lnTerm2;	// log(term2)

    float term2TimesOneOverMaxdLpOneOverGamma;
};

#endif // STARTONEREPRODUCER_H
