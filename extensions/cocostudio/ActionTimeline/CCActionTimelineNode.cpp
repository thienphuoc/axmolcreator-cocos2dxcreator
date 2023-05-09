/****************************************************************************
 Copyright (c) 2013 cocos2d-x.org

 https://axmolengine.github.io/

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "ActionTimeline/CCActionTimelineNode.h"

USING_NS_AX;

NS_TIMELINE_BEGIN

ActionTimelineNode* ActionTimelineNode::create(Node* root, ActionTimeline* action)
{
    ActionTimelineNode* ret = new ActionTimelineNode();
    if (ret->init(root, action))
    {
        ret->autorelease();
        return ret;
    }
    AX_SAFE_DELETE(ret);
    return nullptr;
}

ActionTimelineNode::ActionTimelineNode() : _root(nullptr), _action(nullptr) {}

ActionTimelineNode::~ActionTimelineNode() {}

bool ActionTimelineNode::init()
{
    return Node::init();
}

bool ActionTimelineNode::init(Node* root, ActionTimeline* action)
{
    _root   = root;
    _action = action;

    if (_root)
    {
        _root->removeFromParent();
        addChild(_root);
    }

    return true;
}

void ActionTimelineNode::setRoot(ax::Node* root)
{
    _root = root;
}
ax::Node* ActionTimelineNode::getRoot()
{
    return _root;
}

void ActionTimelineNode::setActionTimeline(ActionTimeline* action)
{
    _action = action;
}
ActionTimeline* ActionTimelineNode::getActionTimeline()
{
    return _action;
}

NS_TIMELINE_END