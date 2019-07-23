#include "Mesh.h"

Mesh::Mesh()
{
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture*> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	setupMesh();
}

Mesh::~Mesh()
{
	for (auto& t : textures) {
		delete t;
	}
	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &VBO);

		glDeleteBuffers(1, &EBO);
}

void Mesh::Draw(Shader shader, glm::vec3 worldPosition)
{
	shader.use();
	// bind appropriate textures
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, worldPosition);

	//unsigned int modelLoc = glGetUniformLocation(shader.id, "model");
	//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	shader.setModel(model);

	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		std::string number;
		std::string name = textures[i]->type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++); // transfer unsigned int to stream
		else if (name == "texture_normal")
			number = std::to_string(normalNr++); // transfer unsigned int to stream
		else if (name == "texture_height")
			number = std::to_string(heightNr++); // transfer unsigned int to stream

												 // now set the sampler to the correct texture unit
		glUniform1i(glGetUniformLocation(shader.id, (name + number).c_str()), i);
		// and finally bind the texture
		//std::cout << glGetUniformLocation(shader.id, (name + number).c_str()) << "\n";
		glBindTexture(GL_TEXTURE_2D, textures[i]->id);
	}

	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);
}

void Mesh::Draw(Shader shader, Texture* texture, glm::vec3 worldPosition, int currentFrame, float playerRotationDegrees)
{
	shader.use();
	// bind appropriate textures
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, worldPosition);
	
	

	if (playerRotationDegrees) { // billboard
		model = glm::rotate(model, glm::radians(playerRotationDegrees + 90.f), glm::vec3(0.0f, -1.0f, 0.0f));
	}


	//model = glm::translate(model, glm::vec3(0.f, 0.f, 0.f));
	
	//unsigned int modelLoc = glGetUniformLocation(shader.id, "model");
	//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	shader.setModel(model);


		glActiveTexture(GL_TEXTURE0); // active proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		std::string number;
		std::string name = texture->type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++); // transfer unsigned int to stream
		else if (name == "texture_normal")
			number = std::to_string(normalNr++); // transfer unsigned int to stream
		else if (name == "texture_height")
			number = std::to_string(heightNr++); // transfer unsigned int to stream

												 // now set the sampler to the correct texture unit
		glUniform1i(glGetUniformLocation(shader.id, (name + number).c_str()), 0);

		//std::cout << this->vertices[1].Position.z << "\n";
		glm::vec2 shift = { (this->vertices[0].TexCoords.x - this->vertices[1].TexCoords.x) * (float)currentFrame, 0.f };
		shader.setVector2("TexCoordShift", shift);
		// and finally bind the texture
		//std::cout << glGetUniformLocation(shader.id, (name + number).c_str()) << "\n";
		glBindTexture(GL_TEXTURE_2D, texture->id);

	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);
}

Mesh Mesh::combine(Mesh& other)
{
	std::vector<Vertex> newVertices;
	std::vector<unsigned int> newIndices;
	std::vector<Texture*> newTextures;
	Mesh newMesh;

	for (auto& index : other.indices) {
		index += this->vertices.size();
	}

	newVertices.insert(newVertices.end(), this->vertices.begin(), this->vertices.end());
	newVertices.insert(newVertices.end(), other.vertices.begin(), other.vertices.end());

	newIndices.insert(newIndices.end(), this->indices.begin(), this->indices.end());
	newIndices.insert(newIndices.end(), other.indices.begin(), other.indices.end());

	newTextures.insert(newTextures.end(), this->textures.begin(), this->textures.end());
	newTextures.insert(newTextures.end(), other.textures.begin(), other.textures.end());

	newMesh = Mesh(newVertices, newIndices, newTextures);
	newMesh.type = this->type;

	//std::cout << newMesh.vertices[0].Normal.x << " " << newMesh.vertices[0].Normal.y << " " << newMesh.vertices[0].Normal.z << "\n";
	//std::cout << newMesh.vertices[1].Normal.x << " " << newMesh.vertices[1].Normal.y << " " << newMesh.vertices[1].Normal.z << "\n";
	//std::cout << newMesh.vertices[2].Normal.x << " " << newMesh.vertices[2].Normal.y << " " << newMesh.vertices[2].Normal.z << "\n";
	//std::cout << newMesh.vertices[3].Normal.x << " " << newMesh.vertices[3].Normal.y << " " << newMesh.vertices[3].Normal.z << "\n";
	//std::cout << "\n";

	return newMesh;
}

void Mesh::changeTexture(glm::vec2 newTexCoords[])
{
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(newTexCoords), newTexCoords);


}

void Mesh::setupMesh()
{
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	// 1. bind Vertex Array Object
	glBindVertexArray(VAO);
	// 2. copy our vertices array in a vertex buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	// 3. copy our index array in a element buffer for OpenGL to use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

	// vertex colours
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Colour));

	// vertex textures
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));


	glBindVertexArray(0);
}
