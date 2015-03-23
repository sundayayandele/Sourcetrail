#include "component/view/GraphViewStyle.h"

#include "utility/logging/logging.h"

#include "settings/ApplicationSettings.h"

GraphViewStyle::NodeMargins::NodeMargins()
	: left(0)
	, right(0)
	, top(0)
	, bottom(0)
	, spacingX(0)
	, spacingY(0)
	, minWidth(0)
	, charWidth(0.0f)
{
}

GraphViewStyle::NodeStyle::NodeStyle()
	: shadowBlurRadius(0)
	, cornerRadius(0)
	, borderWidth(0)
	, borderDashed(false)
	, fontSize(0)
	, fontBold(false)
	, undefinedPattern(false)
{
}

GraphViewStyle::EdgeStyle::EdgeStyle()
	: width(0)
	, zValue(0)
	, isStraight(false)
	, arrowLength(0)
	, arrowWidth(0)
	, arrowClosed(false)
	, cornerRadius(0)
	, verticalOffset(0)
{
}

std::shared_ptr<GraphViewStyleImpl> GraphViewStyle::getImpl()
{
	if (!s_impl)
	{
		LOG_ERROR("No GraphViewStyleImpl instance set.");
		return nullptr;
	}

	return s_impl;
}

void GraphViewStyle::setImpl(std::shared_ptr<GraphViewStyleImpl> impl)
{
	s_impl = impl;
}

float GraphViewStyle::getCharWidthForNodeType(Node::NodeType type)
{
	std::map<Node::NodeType, float>::const_iterator it = s_charWidths.find(type);

	if (it != s_charWidths.end())
	{
		return it->second;
	}

	float charWidth = getImpl()->getCharWidthForNodeType(type);
	s_charWidths.emplace(type, charWidth);
	return charWidth;
}

size_t GraphViewStyle::getFontSizeForNodeType(Node::NodeType type)
{
	switch (type)
	{
	case Node::NODE_UNDEFINED:
	case Node::NODE_NAMESPACE:
		return 12;

	case Node::NODE_UNDEFINED_TYPE:
	case Node::NODE_STRUCT:
	case Node::NODE_CLASS:
	case Node::NODE_ENUM:
	case Node::NODE_TYPEDEF:
	case Node::NODE_TEMPLATE_PARAMETER_TYPE:
	case Node::NODE_FILE:
		return 14;

	case Node::NODE_UNDEFINED_FUNCTION:
	case Node::NODE_UNDEFINED_VARIABLE:
	case Node::NODE_FUNCTION:
	case Node::NODE_METHOD:
	case Node::NODE_GLOBAL_VARIABLE:
	case Node::NODE_FIELD:
	case Node::NODE_ENUM_CONSTANT:
		return 11;
	}
}

size_t GraphViewStyle::getFontSizeOfAccessNode()
{
	return 11;
}

size_t GraphViewStyle::getFontSizeOfNumber()
{
	return 9;
}

std::string GraphViewStyle::getFontNameForNodeType(Node::NodeType type)
{
	return "Source Code Pro";
}

std::string GraphViewStyle::getFontNameOfAccessNode()
{
	return "Myriad Pro";
}

std::string GraphViewStyle::getFontNameOfNumber()
{
	return "Myriad Pro";
}

GraphViewStyle::NodeMargins GraphViewStyle::getMarginsForNodeType(Node::NodeType type, bool hasChildren)
{
	NodeMargins margins;
	margins.spacingX = margins.spacingY = 8;

	switch (type)
	{
	case Node::NODE_UNDEFINED:
	case Node::NODE_NAMESPACE:
		margins.left = margins.right = 15;
		margins.top = 28;
		margins.bottom = 15;
		break;

	case Node::NODE_UNDEFINED_TYPE:
	case Node::NODE_STRUCT:
	case Node::NODE_CLASS:
	case Node::NODE_ENUM:
	case Node::NODE_TYPEDEF:
	case Node::NODE_TEMPLATE_PARAMETER_TYPE:
	case Node::NODE_FILE:
		if (hasChildren)
		{
			margins.left = margins.right = 15;
			margins.top = 30;
			margins.bottom = 10;
		}
		else
		{
			margins.left = margins.right = 8;
			margins.top = margins.bottom = 13;
		}
		break;

	case Node::NODE_UNDEFINED_FUNCTION:
	case Node::NODE_FUNCTION:
	case Node::NODE_METHOD:
		margins.left = margins.right = 5;
		margins.top = margins.bottom = 10;
		break;

	case Node::NODE_UNDEFINED_VARIABLE:
	case Node::NODE_GLOBAL_VARIABLE:
	case Node::NODE_FIELD:
	case Node::NODE_ENUM_CONSTANT:
		margins.left = margins.right = 5;
		margins.top = margins.bottom = 10;
		break;
	}

	margins.charWidth = getCharWidthForNodeType(type);

	return margins;
}

GraphViewStyle::NodeMargins GraphViewStyle::getMarginsOfAccessNode(
	bool expanded, size_t subNodeCount, size_t invisibleSubNodeCount
){
	NodeMargins margins;
	margins.spacingX = margins.spacingY = 8;

	margins.left = margins.right = 10;
	margins.top = 40;

	if (invisibleSubNodeCount == subNodeCount)
	{
		margins.minWidth = 20;
		margins.bottom = 10;
	}
	else
	{
		margins.minWidth = 82;

		if (expanded)
		{
			margins.bottom = 15;
		}
		else if (invisibleSubNodeCount)
		{
			margins.bottom = 23;
		}
		else
		{
			margins.bottom = 10;
		}
	}

	return margins;
}

GraphViewStyle::NodeStyle GraphViewStyle::getStyleForNodeType(
	Node::NodeType type, bool isActive, bool isFocused, bool hasChildren
){
	NodeStyle style;

	style.color = "#D3D3D3";
	style.borderColor = "#00000000";

	style.fontName = getFontNameForNodeType(type);
	style.fontSize = getFontSizeForNodeType(type);

	if (isActive)
	{
		style.borderColor = "#3c3c3c";
	}

	if (isActive || isFocused)
	{
		style.color = ApplicationSettings::getInstance()->getNodeTypeColor(type, "hover");
	}
	else
	{
		style.color = ApplicationSettings::getInstance()->getNodeTypeColor(type);
	}

	switch (type)
	{
	case Node::NODE_UNDEFINED:
		style.borderDashed = true;

	case Node::NODE_NAMESPACE:
		style.borderColor = "#cc8d91";
		style.borderWidth = 1;

		style.cornerRadius = 20;

		style.textOffset.x = 15;
		style.textOffset.y = 6;
		break;

	case Node::NODE_UNDEFINED_TYPE:
		style.undefinedPattern = true;

	case Node::NODE_STRUCT:
	case Node::NODE_CLASS:
	case Node::NODE_ENUM:
	case Node::NODE_TYPEDEF:
	case Node::NODE_TEMPLATE_PARAMETER_TYPE:
	case Node::NODE_FILE:
		if (isActive)
		{
			style.fontBold = true;
		}

		if (isFocused)
		{
			style.shadowColor = "#FF000000";
		}
		else
		{
			style.shadowColor = "#80000000";
		}
		style.shadowBlurRadius = 5;

		if (hasChildren)
		{
			style.cornerRadius = 20;
			style.textOffset.x = 15;
			style.textOffset.y = 8;
		}
		else
		{
			style.cornerRadius = 10;
			style.textOffset.x = 8;
			style.textOffset.y = 4;
		}
		break;

	case Node::NODE_UNDEFINED_FUNCTION:
	case Node::NODE_UNDEFINED_VARIABLE:
		style.undefinedPattern = true;

	case Node::NODE_FUNCTION:
	case Node::NODE_METHOD:
	case Node::NODE_GLOBAL_VARIABLE:
	case Node::NODE_FIELD:
	case Node::NODE_ENUM_CONSTANT:
		if (isActive || isFocused)
		{
			style.fontBold = true;
		}

		style.cornerRadius = 8;
		style.textOffset.x = 5;
		style.textOffset.y = 3;
		break;
	}

	if (isActive)
	{
		style.borderWidth = 1.5f;
	}

	return style;
}

GraphViewStyle::NodeStyle GraphViewStyle::getStyleOfAccessNode()
{
	NodeStyle style;

	style.color = "#FFFFFF";
	style.borderColor = "#00000000";

	style.cornerRadius = 12;

	style.fontName = getFontNameOfAccessNode();
	style.fontSize = getFontSizeOfAccessNode();
	style.fontBold = true;

	style.textOffset.x = 10;
	style.textOffset.y = 10;

	return style;
}

GraphViewStyle::EdgeStyle GraphViewStyle::getStyleForEdgeType(Edge::EdgeType type, bool isActive, bool isFocused)
{
	EdgeStyle style;

	style.width = isActive ? 2 : 1;
	style.zValue = isActive ? 5 : 1;

	style.arrowLength = 5;
	style.arrowWidth = 8;

	style.cornerRadius = 10;
	style.verticalOffset = 2;

	style.originOffset.x = 17;
	style.targetOffset.x = 17;

	style.originOffset.y = -1;
	style.targetOffset.y = 1;

	switch (type)
	{
	case Edge::EDGE_AGGREGATION:
		style.isStraight = true;
		style.color = isActive ? "#DDD" : "#EEE";
		style.width = 1;
		style.zValue = isActive ? -1 : -5;
		break;

	case Edge::EDGE_CALL:
		style.color = "#F1C100";
		style.originOffset.y = 1;
		style.targetOffset.y = -1;
		style.verticalOffset = 4;
		break;
	case Edge::EDGE_USAGE:
		style.color = "#62B29D";
		style.originOffset.y = 3;
		style.targetOffset.y = -3;
		style.verticalOffset = 6;
		break;
	case Edge::EDGE_INHERITANCE:
		style.arrowLength = 15;
		style.arrowWidth = 20;
		style.arrowClosed = true;
		style.targetOffset.x = 29;
	case Edge::EDGE_OVERRIDE:
		style.color = "#CC5E89";
		break;
	case Edge::EDGE_INCLUDE:
		style.color = "#87BA50";
		break;

	default:
		style.color = "#878787";
		break;
	}

	return style;
}

std::map<Node::NodeType, float> GraphViewStyle::s_charWidths;
std::shared_ptr<GraphViewStyleImpl> GraphViewStyle::s_impl;
