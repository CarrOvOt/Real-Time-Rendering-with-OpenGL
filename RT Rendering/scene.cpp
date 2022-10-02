#include "scene.h"
#include "utility/IBL.hpp"

void Scene::setIBL() {

    this->preFilteredCubemap = IBLUtility::PrefilterMapfromEnvCube(this->mainSkybox->GetTextureID());
    this->irradianceCubemap = IBLUtility::GenIrradiance(this->mainSkybox->GetTextureID());
    this->brdfLut = IBLUtility::GenBrdfLut();
}

void Scene::setShaderLight(Shader* shader){

    shader->use();

    for (int i = 0; i < PointLights.size(); ++i) {
        shader->setFloat("point_light[" + std::to_string(i) + "].power", PointLights[i]->Power);
        shader->setVec3("point_light[" + std::to_string(i) + "].pos", glm::vec3(PointLights[i]->Transform[3]));
        shader->setVec3("point_light[" + std::to_string(i) + "].color", PointLights[i]->diffuse);
    }

    for (int i = 0; i < DirLights.size(); ++i) {
        shader->setFloat("dir_light[" + std::to_string(i) + "].power", DirLights[i]->Power);
        shader->setVec3("dir_light[" + std::to_string(i) + "].dir", DirLights[i]->GetDirection());
        shader->setVec3("dir_light[" + std::to_string(i) + "].color", DirLights[i]->diffuse);
    }

    for (int i = 0; i < SpotLights.size(); ++i) {
        shader->setFloat("spot_light[" + std::to_string(i) + "].power", SpotLights[i]->Power);
        shader->setVec3("spot_light[" + std::to_string(i) + "].pos", glm::vec3(SpotLights[i]->Transform[3]));
        shader->setVec3("spot_light[" + std::to_string(i) + "].dir", SpotLights[i]->GetDirection());
        shader->setFloat("spot_light[" + std::to_string(i) + "].cosR1", glm::cos(glm::radians(SpotLights[i]->R1)));
        shader->setFloat("spot_light[" + std::to_string(i) + "].cosR2", glm::cos(glm::radians(SpotLights[i]->R2)));
        shader->setVec3("spot_light[" + std::to_string(i) + "].color", SpotLights[i]->diffuse);
    }

}

void Scene::setShaderCamera(Shader* shader){

    shader->use();

    shader->setMat4("view_sp", this->mainCamera.GetViewMatrix());
    shader->setMat4("proj_sp", this->mainCamera.GetProjMatrix());
    shader->setVec3("camera_pos", this->mainCamera.Position);
    shader->setVec3("camera_dir", this->mainCamera.Front);

}


void Scene::setShaderEnv(Shader* shader){

    unsigned int irNr = 5;
    unsigned int preNr = irNr +1;
    unsigned int brdfNr = irNr+2;

    glActiveTexture(GL_TEXTURE0 + irNr);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->irradianceCubemap);
    shader->setInt("irradianceMap", irNr);

    glActiveTexture(GL_TEXTURE0 + preNr);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->preFilteredCubemap);
    shader->setInt("prefilterMap", preNr);

    glActiveTexture(GL_TEXTURE0 + brdfNr);
    glBindTexture(GL_TEXTURE_2D, this->brdfLut);
    shader->setInt("brdfLUT", brdfNr);
}

void Scene::setShaderOutline() {
    
    outlineShader->use();

    outlineShader->setMat4("view_sp", this->mainCamera.GetViewMatrix());
    outlineShader->setMat4("proj_sp", this->mainCamera.GetProjMatrix());
    outlineShader->setFloat("line_width", this->outline_width);
    outlineShader->setVec3("outline_color", this->outline_color);
}


void Scene::Draw(){
    for (auto model : Models) {
        model->Draw(this->mainShader);
    }
}

void Scene::DrawLights(){
    for (int i = 0; i < PointLights.size(); ++i) {
        PointLights[i]->Draw(this->mainCamera);
    }
    for (int i = 0; i < SpotLights.size(); ++i) {
        SpotLights[i]->Draw(this->mainCamera);
    }
    for (int i = 0; i < DirLights.size(); ++i) {
        DirLights[i]->Draw(this->mainCamera);
    }
    
}

void Scene::DrawSkybox(){
    mainSkybox->Draw(this->mainCamera);
}

void Scene::DrawOutline(){
    for (auto model : Models) {
        model->Draw(this->outlineShader);
    }
}

Scene::~Scene(){

    delete mainShader;
    delete outlineShader;
    delete mainSkybox;

}

