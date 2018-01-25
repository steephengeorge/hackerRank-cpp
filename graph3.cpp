/*
This program is very simple. It contains two classes: Block and Graph.
- Graphs are responsible for ownership of a collection of Blocks.
- Blocks have two pieces of data: A name (std::string) and collection of (std::string) attributes.

This Graph class supports two operations:
1. Add a Block to the Graph (AddBlock)
  - When a Block is added to a Graph, it cannot share a name with any Blocks already in the Graph. If the Block’s name is
    already elsewhere in the graph, its name will be modified to be unique.
  - Calling AddBlock is resource acquisition. When it comes to data management, the Graph is the owner of the Block from
    this point on.
2. Return a collection of every Block in the Graph that has a specified attribute (GetBlocksWithAttribute)
  - Blocks can have any number of attributes.

There are functions below prefixed with Measure_. These _Measure functions exercise the two Graph operations previously
discussed. The goal of this exercise is to optimize the execution time of these functions:
- Measure_AddBlocks
- Measure_FindBlocksByAttribute

If you take a look at main() below, you'll see that it sends the execution time of these functions to stdout. Go ahead and 
run the program as-is and it's obvious that the initial performance of these operations is very bad.

Goal: Let's see how much better you can make it!

Here are the rules:
- The dynamic behavior of the code cannot change. Don't just write code to generate the expected output, expect your solution
  to be tested on different datasets.
- You ARE NOT allowed to modify the Measure_ functions or main().
- You ARE allowed to modify the Block and Graph classes. Feel free to change the API, data structures, etc. as long as you can
  do so without requiring a modification of Measure_ functions or main().
- Feel free to use C++11 language features or any boost classes.

When you're done, please send me three things:
- Your modified optgraph.cpp file. Please add a comment tagging where you've made changes.
- A copy/paste of the program output before and after you made your changes.
- A brief description of the changes that you made.

Bonus exercise:
Are there any problems that you see with this code that aren't related to performance optimization? Any obvious improvements
that jump out to you? Feel free to fix those issues as well, and be sure to include descriptions of what you fixed and why.

Good luck, and feel free to get in touch if you have any specific questions.
*/

//#include <stdio.h>
//#include <tchar.h>
#include <string>
//#include <list>
#include <chrono>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

static const int s_blockCount = 500;
static const int s_attributesCountPerBlock = 100;

class Block
{
public:
    //Block() 
    //{
       //m_attributes.reserve(s_attributesCountPerBlock);       
    //}
    //~Block() { }

    const std::string GetName() { return m_name; }
    void SetName(const std::string& name) { m_name = name; }

    void AddAttribute(const std::string& attribute) { m_attributes.push_back(attribute); }    
    std::vector<std::string> GetAttributes() { return m_attributes; }

private:
    std::string m_name;
    std::vector<std::string> m_attributes;
};

class Graph
{

public:

    Graph()
    {
        m_blocks.reserve(s_blockCount);        
    }

    // Adds the passed block to this graph. Potentially renames the block prior to insertion to guarantee
    // each block in the graph has a unique name.
    void AddBlock(Block* block)
    {
        // Make sure we have a unique name for this block within the content of this Graph object.
        const std::string uniqueName = GetUniqueBlockNameInGraph(block);
        block->SetName(uniqueName);

        // Add it to our collection of blocks.
        m_blocks.push_back(block);
    }
 

    std::vector<Block*> GetBlocksWithAttribute(const std::string& attribute)
    {
        std::vector<Block*> blocksWithAttribute;     
        std::for_each(m_blocks.begin(), m_blocks.end(), [&blocksWithAttribute, &attribute] (Block* b) {
            
            std::vector<std::string> attributes = b->GetAttributes();
            auto found = std::find_if(attributes.begin(),  attributes.end(),
                                        [&attribute](std::string& iterAttribute)
                                            { return   iterAttribute == attribute; });
            if (found != attributes.end())
            {
                blocksWithAttribute.push_back(b);
            }
        });
        return blocksWithAttribute;
    }

private:

    // Determines if the specified blockName is already the name of a block in this Graph.
    bool BlockNameExistsInGraph(const std::string& blockName)
    {        
        auto found = std::find_if(m_blocks.begin(),  m_blocks.end(),
                                        [&blockName](Block* iterBlock)
                                            { return  iterBlock->GetName() == blockName; });
        return  found != m_blocks.end() ? true: false;       
    }

    // Generate a unique name for this block within the graph.
    // If there's no name collision, then the original Block name is returned.
    // If there is a name collision, then we will appends numbers to the name to make unique.
    // So if you call with a block named "a" and there's already a block named "a", then it will return "a1".
    // If there's already an "a1" then it will return "a2", and so on.
    const std::string GetUniqueBlockNameInGraph(Block* block)
    {
        std::string originalName = block->GetName();
        std::string potentialName = block->GetName();
        unsigned int count = 0;
      
        while(BlockNameExistsInGraph(potentialName))
        {           
            potentialName = originalName + std::to_string(count);
            ++count;            
        }
        return potentialName;
    }

private:

    // Here is our collection of blocks in this graph.
    std::vector<Block*> m_blocks;
};


/// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/// DO NOT MODIFY ANY CODE BELOW THIS POINT
/// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

static const int s_blockQuantity = 500;
static const int s_attributesPerBlock = 100;
static std::string s_blockBaseName("a");

std::vector<std::vector<std::string>> GetAttributeData()
{
    std::vector<std::vector<std::string>> attrData;

    for (int i = 0; i < s_blockQuantity; ++i)
    {
        std::vector<std::string> blockAttr;

        for (int j = 0; j < s_attributesPerBlock; j++)
        {
            //std::ostringstream s;
            //s << "attribute_" << j + i;
            std::string attr ("attribute_" + std::to_string(j+1));
            blockAttr.push_back(attr);
        }

        attrData.push_back(blockAttr);
    }

    return attrData;
}

std::chrono::milliseconds Measure_AddBlocks(Graph& graph)
{
    auto attrData = GetAttributeData();

    auto start = std::chrono::system_clock::now();
    for (int i = 0; i < s_blockQuantity; ++i)
    {
        Block* b = new Block;
        b->SetName(s_blockBaseName);
        for (auto a: attrData[i])
        {
            b->AddAttribute(a);
        }
        graph.AddBlock(b);
    }
    auto end = std::chrono::system_clock::now();

    // Post-measurement verification
    

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
}

std::chrono::milliseconds Measure_FindBlocksByAttribute(Graph& graph)
{
    auto start = std::chrono::system_clock::now();
    for (int j = 0; j < s_blockQuantity * 2; j++)
    {
        std::ostringstream s;
        s << "attribute_" << j;
        graph.GetBlocksWithAttribute(s.str()).size();
    }
    auto end = std::chrono::system_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
}

int main()
{
    Graph graph;
    
    auto timeAddBlocks = Measure_AddBlocks(graph);
    std::cout << "Measure_AddBlocks took: " << timeAddBlocks.count() << "ms.\n";

    auto timeFindBocksByAttribute = Measure_FindBlocksByAttribute(graph);
    std::cout << "Measure_FindBlocksByAttribute took: " << timeFindBocksByAttribute.count() << "ms.\n";

    return 0;
}
