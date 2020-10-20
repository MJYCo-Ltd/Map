#ifndef HEADER_CSCENECONE_H
#define HEADER_CSCENECONE_H
#include <Plot/SceneShape/IConeSensor.h>
#include <Inner/ImplSceneSensor.hpp>

class CSceneCone:public ImplSceneSensor<IConeSensor>
{
public:
    CONSTRUCTOR(CSceneCone,ImplSceneSensor<IConeSensor>)

protected:
    /**
     * @brief 更新形状
     */
    void UpdateShape();

    /**
     * @brief 构造模型
     */
    void CreateShape();

    void UpdateColor();

};

#endif // CSCENECONE_H
