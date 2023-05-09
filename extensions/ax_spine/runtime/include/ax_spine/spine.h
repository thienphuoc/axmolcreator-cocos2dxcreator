/******************************************************************************
 * Spine Runtimes License Agreement
 * Last updated May 1, 2019. Replaces all prior versions.
 *
 * Copyright (c) 2013-2019, Esoteric Software LLC
 *
 * Integration of the Spine Runtimes into software or otherwise creating
 * derivative works of the Spine Runtimes is permitted under the terms and
 * conditions of Section 2 of the Spine Editor License Agreement:
 * http://esotericsoftware.com/spine-editor-license
 *
 * Otherwise, it is permitted to integrate the Spine Runtimes into software
 * or otherwise create derivative works of the Spine Runtimes (collectively,
 * "Products"), provided that each user of the Products must obtain their own
 * Spine Editor license and redistribution of the Products in any form must
 * include this license and copyright notice.
 *
 * THIS SOFTWARE IS PROVIDED BY ESOTERIC SOFTWARE LLC "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
 * NO EVENT SHALL ESOTERIC SOFTWARE LLC BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES, BUSINESS
 * INTERRUPTION, OR LOSS OF USE, DATA, OR PROFITS) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

#ifndef SPINE_SPINE_H_
#define SPINE_SPINE_H_

#include <ax_spine/Animation.h>
#include <ax_spine/AnimationState.h>
#include <ax_spine/AnimationStateData.h>
#include <ax_spine/Atlas.h>
#include <ax_spine/AtlasAttachmentLoader.h>
#include <ax_spine/Attachment.h>
#include <ax_spine/AttachmentLoader.h>
#include <ax_spine/AttachmentTimeline.h>
#include <ax_spine/AttachmentType.h>
#include <ax_spine/BlendMode.h>
#include <ax_spine/Bone.h>
#include <ax_spine/BoneData.h>
#include <ax_spine/BoundingBoxAttachment.h>
#include <ax_spine/ClippingAttachment.h>
#include <ax_spine/Color.h>
#include <ax_spine/ColorTimeline.h>
#include <ax_spine/Constraint.h>
#include <ax_spine/ContainerUtil.h>
#include <ax_spine/CurveTimeline.h>
#include <ax_spine/Debug.h>
#include <ax_spine/DeformTimeline.h>
#include <ax_spine/DrawOrderTimeline.h>
#include <ax_spine/Event.h>
#include <ax_spine/EventData.h>
#include <ax_spine/EventTimeline.h>
#include <ax_spine/Extension.h>
#include <ax_spine/HashMap.h>
#include <ax_spine/HasRendererObject.h>
#include <ax_spine/IkConstraint.h>
#include <ax_spine/IkConstraintData.h>
#include <ax_spine/IkConstraintTimeline.h>
#include <ax_spine/Json.h>
#include <ax_spine/LinkedMesh.h>
#include <ax_spine/MathUtil.h>
#include <ax_spine/MeshAttachment.h>
#include <ax_spine/MixBlend.h>
#include <ax_spine/MixDirection.h>
#include <ax_spine/PathAttachment.h>
#include <ax_spine/PathConstraint.h>
#include <ax_spine/PathConstraintData.h>
#include <ax_spine/PathConstraintMixTimeline.h>
#include <ax_spine/PathConstraintPositionTimeline.h>
#include <ax_spine/PathConstraintSpacingTimeline.h>
#include <ax_spine/PointAttachment.h>
#include <ax_spine/Pool.h>
#include <ax_spine/PositionMode.h>
#include <ax_spine/RegionAttachment.h>
#include <ax_spine/RotateMode.h>
#include <ax_spine/RotateTimeline.h>
#include <ax_spine/RTTI.h>
#include <ax_spine/ScaleTimeline.h>
#include <ax_spine/ShearTimeline.h>
#include <ax_spine/Skeleton.h>
#include <ax_spine/SkeletonBinary.h>
#include <ax_spine/SkeletonBounds.h>
#include <ax_spine/SkeletonClipping.h>
#include <ax_spine/SkeletonData.h>
#include <ax_spine/SkeletonJson.h>
#include <ax_spine/Skin.h>
#include <ax_spine/Slot.h>
#include <ax_spine/SlotData.h>
#include <ax_spine/SpacingMode.h>
#include <ax_spine/SpineObject.h>
#include <ax_spine/SpineString.h>
#include <ax_spine/TextureLoader.h>
#include <ax_spine/Timeline.h>
#include <ax_spine/TimelineType.h>
#include <ax_spine/TransformConstraint.h>
#include <ax_spine/TransformConstraintData.h>
#include <ax_spine/TransformConstraintTimeline.h>
#include <ax_spine/TransformMode.h>
#include <ax_spine/TranslateTimeline.h>
#include <ax_spine/Triangulator.h>
#include <ax_spine/TwoColorTimeline.h>
#include <ax_spine/Updatable.h>
#include <ax_spine/Vector.h>
#include <ax_spine/VertexAttachment.h>
#include <ax_spine/VertexEffect.h>
#include <ax_spine/Vertices.h>

#endif
