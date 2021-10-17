#include "Model.h"

#include "IL/il.h"
#include "IL/ilut.h"
#include "Globals.h"
#include "mmgr/mmgr.h"

Model::Model(const char* path)
{
	LoadModel(path);
}

Model::~Model()
{
}

void Model::Draw()
{
	// Loop all the meshes in our model and draw them
	for (unsigned int i = 0; i < meshes.size(); ++i)
	{
		meshes[i].Draw();
	}
}

void Model::LoadModel(std::string path)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		DEBUG_LOG("ERROR ASSIMP %s", import.GetErrorString());
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	// Process first all the meshes in our current root node
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene));
	}

	// Repeat the process until there's no more children
	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		ProcessNode(node->mChildren[i], scene);
	}

}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	std::vector<float2> texCoords;

	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
	{
		Vertex vertex;
		vertex.position.x = mesh->mVertices[i].x;
		vertex.position.y = mesh->mVertices[i].y;
		vertex.position.z = mesh->mVertices[i].z;

		vertex.normal.x = mesh->mNormals[i].x;
		vertex.normal.y = mesh->mNormals[i].y;
		vertex.normal.z = mesh->mNormals[i].z;
		
		float2 coords;
		coords.x = mesh->mTextureCoords[0][i].x;
		coords.y = mesh->mTextureCoords[0][i].y;
		
		vertices.push_back(vertex);
		texCoords.push_back(coords);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	Texture diffuse;
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		diffuse = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		
		//std::vector<Texture> specular = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		//textures.insert(textures.end(), specular.begin(), specular.end());
	}

	return Mesh(vertices, indices, diffuse, texCoords);
}

Texture Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const char* typeName)
{
	Texture texture = {};
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		std::string path = ASSETS_FOLDER;
		path += str.C_Str();
		ILuint image;
		ilGenImages(1, &image);
		ilBindImage(image);
		ilLoadImage(ASSETS_FOLDER "Lenna.png");
		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		//texture.id = ilutGLLoadImage(ASSETS_FOLDER "Lenna.png");
		texture.width = ilGetInteger(IL_IMAGE_WIDTH);
		texture.height = ilGetInteger(IL_IMAGE_HEIGHT);
		texture.id = image;
	}

	return texture;
}