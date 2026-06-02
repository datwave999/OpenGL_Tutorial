#include "Mesh.h"

Mesh::Mesh() {
	VAO = 0;
	VBO = 0;
	IBO = 0;
	indexCount = 0;
}

void Mesh::CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices) {
	indexCount = numOfIndices;
	 
	//Generates 1 VAO on the GPU and puts the ID in the variable VAO
	glGenVertexArrays(1, &VAO);
	//Binds it, means we are using this VAO currently
	glBindVertexArray(VAO);

	//Creating IBO & Binding
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	//Feeding index values to IBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW);

	//Similar to VAO
	glGenBuffers(1, &VBO);
	//Multiple buffer types are there, we are currently binding VBO to GL_ARRAY type
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Now that VBO is binded to ARRAY_BUFFER, we give it vertices and specify kind of draw\
	//STATIC_DRAW = value of vertices wont change
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW);

	//Pointing (location in shader, coordinates of a single vertex, data type, normalization, stride, offset)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, 0);
	//Enabling location 0
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 3));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 5));
	glEnableVertexAttribArray(2);

	//Unbinding
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//Unbind IBO after unbinding VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return;
}

void Mesh::RenderMesh() {
	if (indexCount <= 0) return;
	//Using the VAO to draw
	glBindVertexArray(VAO);
	//The Draw Function
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::ClearMesh() {
	if (IBO != 0) {
		glDeleteBuffers(1, &IBO);
		IBO = 0;
	}
	if (VBO != 0) {
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}
	if (VAO != 0) {
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}
}

Mesh::~Mesh() {
	ClearMesh();
}