/****************************************************************************
 Copyright (c) 2014-2016 Chukong Technologies Inc.
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

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

#ifndef __CCATTACHNODE_H__
#define __CCATTACHNODE_H__

#include "axmol/core/math/CCMath.h"
#include "2d/CCNode.h"

NS_AX_BEGIN
/**
 * @addtogroup _3d
 * @{
 */
class Bone3D;

/**
 * @brief attach a node to a bone
 * usage: auto mesh = MeshRenderer::create("girl.c3b");
 *        auto weapon = MeshRenderer::create("weapon.c3b");
 *        auto attachNode = mesh->getAttachNode("left hand");
 *        attachNode->addChild(weapon);
 */
class AX_DLL AttachNode : public Node
{
public:
    /**
     * creates an AttachNode
     * @param attachBone The bone to which the AttachNode is going to attach, the attacheBone must be a bone of the
     * AttachNode's parent
     */
    static AttachNode* create(Bone3D* attachBone);

    // override
    virtual Mat4 getWorldToNodeTransform() const override;
    virtual Mat4 getNodeToWorldTransform() const override;
    virtual const Mat4& getNodeToParentTransform() const override;
    virtual void visit(Renderer* renderer, const Mat4& parentTransform, uint32_t parentFlags) override;

    AttachNode();
    virtual ~AttachNode();

protected:
    Bone3D* _attachBone;
    mutable Mat4 _transformToParent;
};

// end of 3d group
/// @}

NS_AX_END
#endif  // __CCATTACHNODE_H__