#include "DPBase.h"
#include "DPJsonParser.h"

using namespace DevilsParser;

int main(void)
{
	
	const char* path = "F:\\DevilsParser\\DevilsParser\\JSONTestFiles\\ProductionJSON\\ProdBase.json";
	DPJsonParser* parser = new DPJsonParser(path, DPJsonParser::ParserMode::eRead);

	parser->Parse();
	
	DPJsonNodeData data;
	ReturnStatus rs = parser->Query("properties.name.description", data);

	if (parser)
		delete parser;
	parser = NULL;

	return 0;
}
