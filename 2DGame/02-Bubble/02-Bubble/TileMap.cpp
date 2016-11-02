#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"
#include "Define.h"
#include "Game.h"


using namespace std;


TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	TileMap *map = new TileMap(levelFile, minCoords, program);
	return map;
}


TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	loadLevel(levelFile);
	this->vertices.reserve(mapSize.x * mapSize.y * 24);
	this->nTiles = 0;
	this->program = program;
	prepareArrays(minCoords);
	
}

TileMap::~TileMap()
{
	if (map != NULL)
		delete map;
}


void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * nTiles);
	glDisable(GL_TEXTURE_2D);
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string &levelFile)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	char tile;
	
	fin.open(levelFile.c_str());
	if(!fin.is_open())
		return false;
	getline(fin, line);
	if(line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize >> blockSize;
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);
	
	map = new Tile[mapSize.x * mapSize.y];
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			fin.get(tile);
			if (tile == ' '){
				map[j*mapSize.x + i].id = 0;
			}
			else{
				map[j*mapSize.x + i].id = tile - int('0');
				if (map[j*mapSize.x + i].id != 0);
			}
			map[j*mapSize.x + i].vertexIndex = -1;
			map[j*mapSize.x + i].vida = 50;

		}
		fin.get(tile);
#ifndef _WIN32
		fin.get(tile);
#endif
	}
	fin.close();
	
	return true;
}



void TileMap::prepareArrays(const glm::vec2 &minCoords)
{
	position = glm::vec2(minCoords);
	
	Tile tile;

	for(int y=0; y<mapSize.y; y++)
	{
		for(int x=0; x<mapSize.x; x++)
		{
			tile = map[y * mapSize.x + x];
			
			setTile(tile.id, x, y, false);
		}
	}
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	setBufferData();

}

void TileMap::setWorldTile(int id, double x, double y) {
	int xIndex = (x - position.x) / tileSize;
	int yIndex = (y - position.y) / tileSize;


	if (xIndex < mapSize.x && yIndex < mapSize.y) {
		setTile(id, xIndex, yIndex, true);
	}
}


void TileMap::setTile(int id, int x, int y, bool redraw) {
	int index = y * mapSize.x + x;
	//("Index: %d\n", index);
	int idaux = map[index].id;
	Tile *t = &map[index];

	if (t->vertexIndex != -1) {
		if (t->vida > 0){
			t->vida--;
		}
		else{
			//Tree TODO
			

			//Fer aixo x afegir les fustes
			/*for (int i = 0; i < 3; i++)
				Game::instance().getScene()->getInventary()->putItem(WOOD, Game::instance().getScene()->getInventary()->getFirstEmptySlot(), program);
			Game::instance().getScene()->getCrafting()->update();
			Game::instance().getScene()->setPlayerItem(Game::instance().getScene()->getActiveItem());*/

			switch (t->id){
			case TILE_GOLD1:
			case TILE_GOLD2:
				Game::instance().getScene()->getInventary()->putItem(GOLD_BAR, Game::instance().getScene()->getInventary()->getFirstEmptySlot(), program);
				Game::instance().getScene()->getCrafting()->update();
				Game::instance().getScene()->setPlayerItem(Game::instance().getScene()->getActiveItem());
				break;
			case TILE_BRONZE1:
			case TILE_BRONZE2:
				Game::instance().getScene()->getInventary()->putItem(BRONZE_BAR, Game::instance().getScene()->getInventary()->getFirstEmptySlot(), program);
				Game::instance().getScene()->getCrafting()->update();
				Game::instance().getScene()->setPlayerItem(Game::instance().getScene()->getActiveItem());
				break;
			case TILE_DIAMOND1:
			case TILE_DIAMOND2:
				Game::instance().getScene()->getInventary()->putItem(TILE_DIAMOND1, Game::instance().getScene()->getInventary()->getFirstEmptySlot(), program);
				Game::instance().getScene()->getCrafting()->update();
				Game::instance().getScene()->setPlayerItem(Game::instance().getScene()->getActiveItem());
				break;
			default:
				cout << "TILE ARBRE " << t->id << endl;
				break;
			}

			for (size_t i = 0; i < mapSize.x * mapSize.y; i++) {
				if (map[i].vertexIndex > t->vertexIndex) {
					map[i].vertexIndex -= 24;
				}
			}

			vector<float>::iterator start = vertices.begin() + t->vertexIndex;
			vector<float>::iterator end = start + 24;
			vertices.erase(start, end);
			t->vertexIndex = -1;
			nTiles--;
			
			t->id = id;
		}
	}


	if (id != 0) {
		glm::vec2 texCoordTile[2], halfTexel;
		halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
		glm::vec2 posTile = glm::vec2(position.x + x * tileSize, position.y + y * tileSize);
		texCoordTile[0] = glm::vec2(float((t->id - 1) % 2) / tilesheetSize.x, float((t->id - 1) / 2) / tilesheetSize.y);
		texCoordTile[1] = texCoordTile[0] + tileTexSize;
		//texCoordTile[0] += halfTexel;
		texCoordTile[1] -= halfTexel;

		nTiles++;
		t->vertexIndex = vertices.size();
		// First triangle
		vertices.push_back(posTile.x); vertices.push_back(posTile.y);
		vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
		vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
		vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
		vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
		vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
		// Second triangle
		vertices.push_back(posTile.x); vertices.push_back(posTile.y);
		vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
		vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
		vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
		vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
		vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
	}

	

	if (redraw) {
		setBufferData();
	}
}


void TileMap::setBufferData() {

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void *)(2 * sizeof(float)));
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

bool TileMap::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;
	
	x = pos.x / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for(int y=y0; y<=y1; y++)
	{
		if (map[y*mapSize.x + x].id != 0 && map[y*mapSize.x + x].id < 'A'-'0')
			return true;
	}
	
	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;
	
	x = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for(int y=y0; y<=y1; y++)
	{
		if (map[y*mapSize.x + x].id != 0 && map[y*mapSize.x + x].id < 'A'-'0')
			return true;
	}
	
	return false;
}

bool TileMap::collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const
{
	int x0, x1, y;
	
	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;
	for(int x=x0; x<=x1; x++)
	{
		if (map[y*mapSize.x + x].id != 0 && map[y*mapSize.x + x].id < 'A'-'0')
		{
			if(*posY - tileSize * y + size.y <= 8)
			{
				*posY = tileSize * y - size.y;
				return true;
			}
		}
	}
	
	return false;
}

bool TileMap::collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = pos.y / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		if (map[y*mapSize.x + x].id != 0 && map[y*mapSize.x + x].id < 'A'-'0')
		{
			return true;
		}
	}

	return false;
}



