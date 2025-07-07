#include "modelgl.h"

void ModelGL::setModelData(const ModelData *modelData)
{
    qDebug() << "[DEBUG] ModelGL::setModelData: Установка данных модели";

    if (!modelData) {
        qDebug() << "[ERROR] ModelGL::setModelData: nullptr передан как модель";
        return;
    }

    m_modelData = *modelData;

    qDebug() << "[INFO] Вершин:" << m_modelData.vertices().size();
    qDebug() << "[INFO] Индексов граней:" << m_modelData.faceVertexIndices().size();
    qDebug() << "[INFO] Текстурных координат:" << m_modelData.textureVertices().size();
    qDebug() << "[INFO] Нормалей:" << m_modelData.normals().size();
}

const ModelData *ModelGL::getModelData() const
{
    qDebug() << "[DEBUG] ModelGL::getModelData: Возвращается указатель на данные модели";
    return &m_modelData;
}

GLuint ModelGL::vao() const
{
    return m_vao;
}

GLuint ModelGL::vertexCount() const
{
    return m_vertexCount;
}

GLuint ModelGL::textureId() const
{
    return m_textureId;
}

void ModelGL::setVao(GLuint vao)
{
    qDebug() << "[DEBUG] ModelGL::setVao: установлено значение" << vao;
    m_vao = vao;
}

void ModelGL::setVertexCount(GLuint count)
{
    qDebug() << "[DEBUG] ModelGL::setVertexCount: установлено количество вершин" << count;
    m_vertexCount = count;
}

bool ModelGL::hasTexture() const
{
    qDebug() << "[DEBUG] ModelGL::hasTexture: текстура" << (m_hasTexture ? "включена" : "выключена");
    return m_hasTexture;
}

bool ModelGL::useNormals() const
{
    qDebug() << "[DEBUG] ModelGL::useNormals: использование нормалей" << (m_useNormals ? "включено" : "выключено");
    return m_useNormals;
}

bool ModelGL::isValid() const
{
    bool valid = !m_modelData.vertices().isEmpty() && !m_modelData.faceVertexIndices().isEmpty();

    qDebug() << "[DEBUG] ModelGL::isValid: проверка валидности модели";
    qDebug() << " - Вершины присутствуют:" << !m_modelData.vertices().isEmpty();
    qDebug() << " - Индексы граней присутствуют:" << !m_modelData.faceVertexIndices().isEmpty();
    qDebug() << " - Модель валидна:" << valid;

    return valid;
}

bool ModelGL::hasNormals() const
{
    qDebug() << "[DEBUG] ModelGL::hasNormals: наличие нормалей" << !m_modelData.normals().isEmpty();
    return !m_modelData.normals().isEmpty();
}

void ModelGL::setHasTexture(bool enabled)
{
    qDebug() << "[DEBUG] ModelGL::setHasTexture: установка значения" << (enabled ? "true" : "false");
    m_hasTexture = enabled;
}

void ModelGL::setUseNormals(bool enabled)
{
    qDebug() << "[DEBUG] ModelGL::setUseNormals: установка значения" << (enabled ? "true" : "false");
    m_useNormals = enabled;
}

void ModelGL::setTextureId(GLuint id)
{
    qDebug() << "[DEBUG] ModelGL::setTextureId: установлен ID текстуры" << id;
    m_textureId = id;
}

void ModelGL::clear()
{
    qDebug() << "[DEBUG] ModelGL::clear: очистка модели";

    m_vao = 0;
    m_vertexCount = 0;
    m_textureId = 0;
    m_hasTexture = false;
    m_useNormals = false;
    m_modelData = ModelData();
}

bool ModelGL::operator!=(const ModelGL &other) const
{
    bool result = m_vao != other.m_vao ||
                  m_vertexCount != other.m_vertexCount ||
                  m_textureId != other.m_textureId ||
                  m_hasTexture != other.m_hasTexture ||
                  m_useNormals != other.m_useNormals;

    qDebug() << "[DEBUG] ModelGL::operator!=: результат сравнения моделей" << (result ? "true (разные)" : "false (одинаковые)");

    return result;
}
