#include "scene.h"
#include "utility/IBL.h"


void Scene::setShaderLight(){

    mainShader.use();

    mainShader.setFloat("point_light.power", PointLights[0].Power);
    mainShader.setVec3("point_light.pos", glm::vec3(PointLights[0].Transform[3]));
    mainShader.setVec3("point_light.color", PointLights[0].diffuse);
    
    mainShader.setFloat("dir_light.power", DirLights[0].Power);
    mainShader.setVec3("dir_light.dir", DirLights[0].GetDirection());
    mainShader.setVec3("dir_light.color", DirLights[0].diffuse);
    
    mainShader.setFloat("spot_light.power", SpotLights[0].Power);
    mainShader.setVec3("spot_light.pos", glm::vec3(SpotLights[0].Transform[3]));
    mainShader.setVec3("spot_light.dir", SpotLights[0].GetDirection());
    mainShader.setFloat("spot_light.cosR1", glm::cos(glm::radians(SpotLights[0].R1)));
    mainShader.setFloat("spot_light.cosR2", glm::cos(glm::radians(SpotLights[0].R2)));
    mainShader.setVec3("spot_light.color", SpotLights[0].diffuse);

}

void Scene::setShaderCamera(){

    mainShader.use();

    mainShader.setMat4("view_sp", this->mainCamera.GetViewMatrix());
    mainShader.setMat4("proj_sp", this->mainCamera.GetProjMatrix());
    mainShader.setVec3("camera_pos", this->mainCamera.Position);
    mainShader.setVec3("camera_dir", this->mainCamera.Front);

}

void Scene::setIBL(){

    this->preFilteredCubemap = PrefilterMapfromEnvCube(this->mainSkybox.GetTextureID());
    this->irradianceCubemap = GenIrradiance(this->mainSkybox.GetTextureID());
    this->brdfLut = GenBrdfLut();
}

void Scene::setShaderEnv(){

    unsigned int irNr = 4;
    unsigned int preNr = 5;
    unsigned int brdfNr = 6;

    glActiveTexture(GL_TEXTURE0 + irNr);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->irradianceCubemap);
    mainShader.setInt("irradianceMap", irNr);

    glActiveTexture(GL_TEXTURE0 + preNr);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->preFilteredCubemap);
    mainShader.setInt("prefilterMap", preNr);

    glActiveTexture(GL_TEXTURE0 + brdfNr);
    glBindTexture(GL_TEXTURE_2D, this->brdfLut);
    mainShader.setInt("brdfLUT", brdfNr);
}

void Scene::setShaderOutline() {
    
    outlineShader.use();

    outlineShader.setMat4("view_sp", this->mainCamera.GetViewMatrix());
    outlineShader.setMat4("proj_sp", this->mainCamera.GetProjMatrix());
    outlineShader.setFloat("line_width", this->outline_width);
    outlineShader.setVec3("outline_color", this->outline_color);
}


void Scene::Draw(){
    for (auto model : Models) {
        model.Draw(this->mainShader);
    }
}

void Scene::DrawLights(){
    SpotLights[0].Draw(this->mainCamera);
    DirLights[0].Draw(this->mainCamera);
    PointLights[0].Draw(this->mainCamera);
}

void Scene::DrawSkybox(){
    mainSkybox.Draw(this->mainCamera);
}

void Scene::DrawOutline(){
    for (auto model : Models) {
        model.Draw(this->outlineShader);
    }
}
