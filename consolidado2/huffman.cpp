#include "../PixelLab/src/PixelLab/pixelLab.h"
#include <algorithm>
#include <functional>
#include <ctype.h>
#include <stdio.h>
#include <GL/glut.h>
#include <GL/glui.h>
#include <list>
#include <vector>
#include <iostream>

#include <queue>
#include <map>
#include <iterator>

using namespace std;

struct element
{
	pixel pix;
	int occurrence;

    bool operator < (const element& str) const
    {
        return (occurrence < str.occurrence);
    }
};

bool compareColor(pixel corA, pixel corB){
	return corA.R == corB.R && corA.G == corB.G && corA.B == corB.B;
}

//---------------- huffmam code stuff------------
typedef vector<bool> HuffCode;
typedef map<char, HuffCode> HuffCodeMap;
 
class INode
{
public:
    const int f;
 
    virtual ~INode() {}
 
protected:
    INode(int f) : f(f) {}
};
 
class InternalNode : public INode
{
public:
    INode *const left;
    INode *const right;
 
    InternalNode(INode* c0, INode* c1) : INode(c0->f + c1->f), left(c0), right(c1) {}
    ~InternalNode()
    {
        delete left;
        delete right;
    }
};
 
class LeafNode : public INode
{
public:
    const char c;
 
    LeafNode(int f, char c) : INode(f), c(c) {}
};
 
struct NodeCmp
{
    bool operator()(const INode* lhs, const INode* rhs) const { return lhs->f > rhs->f; }
};

INode* BuildTree(const vector<element> ptr)
{
    priority_queue<INode*, vector<INode*>, NodeCmp> trees;
 
    for (int i = 0; i < ptr.size(); ++i)
    {
        if(ptr[i].occurrence != 0)
            trees.push(new LeafNode(ptr[i].occurrence, (char)i));
    }
    while (trees.size() > 1)
    {
        INode* childR = trees.top();
        trees.pop();
 
        INode* childL = trees.top();
        trees.pop();
 
        INode* parent = new InternalNode(childR, childL);
        trees.push(parent);
    }
    return trees.top();
}

void GenerateCodes(const INode* node, const HuffCode& prefix, HuffCodeMap& outCodes)
{
    if (const LeafNode* lf = dynamic_cast<const LeafNode*>(node))
    {
        outCodes[lf->c] = prefix;
    }
    else if (const InternalNode* in = dynamic_cast<const InternalNode*>(node))
    {
        HuffCode leftPrefix = prefix;
        leftPrefix.push_back(false);
        GenerateCodes(in->left, leftPrefix, outCodes);
 
        HuffCode rightPrefix = prefix;
        rightPrefix.push_back(true);
        GenerateCodes(in->right, rightPrefix, outCodes);
    }
}
//------------------------------------------

void huffman(PixelLab* img) { 
	int var;
	element elmt;
	vector<element> patter;
	pixel cor;
	img->GetRGB(0, 0, cor.R, cor.G, cor.B);
	elmt.pix = cor;
	elmt.occurrence = 0;
	patter.push_back(elmt);

	bool test = false;
	int x;

	cout << "percorre Imagem" << endl;
	for (int i = 0; i < img->GetHeight(); i++) {
		for (int j = 0; j < img->GetWidth(); j++) {
			pixel cor;
			img->GetRGB(i, j, cor.R, cor.G, cor.B);
			test = false;
			for(x = 0; x < patter.size(); x++){
				if (compareColor(cor, patter[x].pix)){
		      		test = true;
		      		break;
			    }
		   	}

		   	if (test){
	      		patter[x].occurrence +=1;
		    } else {
		    	element elmt;
		    	elmt.pix = cor;
				elmt.occurrence = 1;
				patter.push_back(elmt);
		    }
		}
	}
	
	sort(patter.begin(), patter.end());
	
	INode* root = BuildTree(patter);
	
	HuffCodeMap codes;
    GenerateCodes(root, HuffCode(), codes);
    delete root;
    cout << "GenerateCodes" << endl;

    ofstream outfile ("huffman.dat", ofstream::binary);

    for (HuffCodeMap::const_iterator it = codes.begin(); it != codes.end(); ++it) {
        outfile.write(reinterpret_cast<const char *>(&(it->first)), sizeof(it->first));
        for (int i = 0; i < it->second.size(); ++i) {
            //outfile.write(reinterpret_cast<const char *>(&(it->second[i])), sizeof(it->second[i]));
            int aux = it->second[i];
            outfile.write(reinterpret_cast<const char *>(&aux), sizeof(it->second[i]));
        }
    }

    outfile.close();

    //double originalSize = img->GetHeight() *img->GetWidth() * sizeof(uByte) * 3;

    //cout << "Tamanho da imagem original: " << (originalSize / 1024.0) << " KB\n";

    //cout << "tamanho compactado " << (sizeCompactado / 1024.0) << " KB\n" ;
}