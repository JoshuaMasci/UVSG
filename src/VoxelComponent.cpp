#include "VoxelComponent.hpp"



VoxelComponent::VoxelComponent()
{
	for (unsigned int x = 0; x < chunkSize; x++)
	{
		for (unsigned int y = 0; y < chunkSize; y++)
		{
			for (unsigned int z = 0; z < chunkSize; z++)
			{
				collisionChunk[x][y][z] = 0;
			}
		}
	}
};

VoxelComponent::~VoxelComponent()
{
	printf("Voxel Death!!!\n");
}

bool VoxelComponent::getShouldUpdateChunk()
{
	return shouldUpdateChunk;
}

void VoxelComponent::setShouldUpdateChunk()
{
	shouldUpdateChunk = true;
}

void VoxelComponent::clearShouldUpdateChunk()
{
	shouldUpdateChunk = false;
}

void VoxelComponent::setBlock(int x, int y, int z, BlockID id)
{
	if (x >= 0 && x < chunkSize)
	{
		if (y >= 0 && y < chunkSize)
		{
			if (z >= 0 && z < chunkSize)
			{
				chunk[x][y][z] = id;
				shouldUpdateChunk = true;
			}
		}
	}
}

BlockID VoxelComponent::getBlock(int x, int y, int z)
{
	if (x >= 0 && x < chunkSize)
	{
		if (y >= 0 && y < chunkSize)
		{
			if (z >= 0 && z < chunkSize)
			{
				return chunk[x][y][z];
			}
		}
	}
	return 0;
}