#include "renderer.h"

#include <QDebug>
#include <QImage>

namespace Viewer
{
bool OpenGLRenderer::initialize()
{
    qDebug() << "OpenGLRenderer :: initialize : запустили метод initialize";

    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);



    shaderProgram = new ShaderProgram();

    if (!shaderProgram->compileFromString(
                R"(
                #version 330 core

                layout(location=0) in vec3 position;
                layout(location=1) in vec3 normal;
                layout(location=2) in vec2 inTexCoord;

                uniform mat4 openGLcurrentMvp;

                out vec2 outTexCoord;
                out vec3 fragNormal;

                void main()
                {
                    fragNormal = normal;
                    outTexCoord = inTexCoord;
                    gl_Position = openGLcurrentMvp * vec4(position, 1.0);
                }
                )",
                R"(
                #version 330 core

                in vec2 outTexCoord;
                in vec3 fragNormal;

                out vec4 outColor;

                uniform sampler2D modelTexture;
                uniform bool useTexture;
                uniform bool useNormal;

                uniform vec3 lightDirection = vec3(0.5, 0.5, 1.0);
                uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);
                uniform vec3 objectColor = vec3(0.8, 0.5, 0.3);

                void main()
                {
                    vec3 color = objectColor;

                    if (useTexture) {
                        color = texture(modelTexture, outTexCoord).rgb;
                    }

                    if (useNormal) {
                        vec3 norm = normalize(fragNormal);
                        vec3 lightDir = normalize(-lightDirection);
                        float diff = max(dot(norm, lightDir), 0.0);
                        vec3 diffuse = diff * lightColor;
                        color = diffuse * color;
                    }

                    outColor = vec4(color, 1.0);
                }
                )")) {

        qCritical("Failed to compile shaders");
        delete shaderProgram;
        shaderProgram = nullptr;
        return false;
    }
    openGLisInitialized = true;
    return true;
}

bool OpenGLRenderer::initializeModel(Model &model)
{
    qDebug() << "OpenGLRenderer :: initializeModel : запустили метод initializeModel";

    if (!openGLisInitialized || model.m_vertices.isEmpty()) {
        qDebug() << "Досрочное завершение из-за openGLisInitialized или отсутствия вершин";
        return false;
    }

    if (model.m_vao != 0){
        glDeleteVertexArrays(1, &model.m_vao);
        model.m_vao = 0;
    }

    glGenVertexArrays(1, &model.m_vao);
    glBindVertexArray(model.m_vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    QVector<float> fullData;

    for (int i = 0; i < model.m_faceVertexIndices.size(); ++i) {
        int idx = model.m_faceVertexIndices[i];

        QVector3D pos = model.m_vertices.value(idx, QVector3D());
        fullData << pos.x() << pos.y() << pos.z();

        QVector3D normal = model.m_normals.value(idx, QVector3D());
        fullData << normal.x() << normal.y() << normal.z();

        if (i < model.m_faceTextureVertexIndices.size()) {
            int texIdx = model.m_faceTextureVertexIndices[i];
            if (texIdx >= 0 && texIdx < model.m_textureVertices.size()) {
                QVector2D tex = model.m_textureVertices[texIdx];
                fullData << tex.x() << tex.y();
            } else {
                fullData << 0.0f << 0.0f;
            }
        } else {
            fullData << 0.0f << 0.0f;
        }
    }

    glBufferData(GL_ARRAY_BUFFER, fullData.size() * sizeof(float), fullData.constData(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    model.m_vertexCount = model.m_faceVertexIndices.size();

    glBindVertexArray(0);

    qDebug() << "[DEBUG] VAO создан для модели";
    qDebug() << "Количество вершин:" << model.m_vertexCount;

    return true;
}

void OpenGLRenderer::render(const Model& model, const QMatrix4x4& mvp)
{
    if (!openGLisInitialized || !shaderProgram || !model.isValid()) {
            return;
        }

        shaderProgram->get()->bind();
        shaderProgram->get()->setUniformValue("openGLcurrentMvp", mvp);
        shaderProgram->get()->setUniformValue("useNormal", model.m_useNormals);
        shaderProgram->get()->setUniformValue("useTexture", model.m_hasTexture);

        if (model.m_vao == 0) {
            qDebug() << "VAO не создан — создаём сейчас";
            if (!initializeModel(const_cast<Model&>(model))) {
                shaderProgram->get()->release();
                return;
            }
        }

        glBindVertexArray(model.m_vao);

        if (model.m_hasTexture) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, model.m_textureId);
            shaderProgram->get()->setUniformValue("modelTexture", 0);
        }

        for (int i = 0; i < model.m_polygonStarts.size(); ++i) {
            int start = model.m_polygonStarts[i];
            int count = (i < model.m_polygonStarts.size() - 1)
                            ? model.m_polygonStarts[i + 1] - start
                            : model.m_faceVertexIndices.size() - start;

            glDrawArrays(GL_TRIANGLES, start, count);
        }

        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
        shaderProgram->get()->release();

    qDebug() << "[DEBUG] Модель отрисована";
    qDebug() << "hasTexture:" << model.m_hasTexture;
    qDebug() << "textureId:" << model.m_textureId;
}

bool OpenGLRenderer::loadTexture(Model &model, const QString &texturePath)
{
    if (!openGLisInitialized) {
        qDebug() << "[ERROR] OpenGL не инициализирован";
        return false;
    }

    QImage textureImage(texturePath);
    if (textureImage.isNull()) {
        qWarning() << "Не удалось загрузить текстуру:" << texturePath;
        return false;
    }

    textureImage = textureImage.convertToFormat(QImage::Format_RGBA8888).mirrored();

    if (textureImage.width() <= 0 || textureImage.height() <= 0) return false;

    glGenTextures(1, &model.m_textureId);
    glBindTexture(GL_TEXTURE_2D, model.m_textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureImage.width(), textureImage.height(),
                  0, GL_RGBA, GL_UNSIGNED_BYTE, textureImage.bits());

    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    model.m_hasTexture = true;

    qDebug() << "[SUCCESS] Текстура загружена:" << texturePath;
    qDebug() << "Размер текстуры:" << textureImage.width() << "x" << textureImage.height();
    qDebug() << "textureId:" << model.m_textureId;

    return true;
}

void OpenGLRenderer::setModel(const Model& model)
{
    qDebug() << "OpenGLRenderer :: setModel : запустили метод setModel";
    openGLcurrentModel = model;
    initializeModel(openGLcurrentModel);
}

void OpenGLRenderer::setMVPmatrix(const QMatrix4x4& mvp)
{
    qDebug() << "OpenGLRenderer :: setViewProjectionMatrix : запустили метод setViewProjectionMatrix";
    openGLcurrentMvp = mvp;
    qDebug() << "openGLcurrentMvp: " << openGLcurrentMvp;
}
}
