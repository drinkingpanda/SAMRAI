/*************************************************************************
 *
 * This file is part of the SAMRAI distribution.  For full copyright
 * information, see COPYRIGHT and COPYING.LESSER.
 *
 * Copyright:     (c) 1997-2011 Lawrence Livermore National Security, LLC
 * Description:   For describing coarse-fine boundary interfaces
 *
 ************************************************************************/

#ifndef included_hier_CoarseFineBoundary
#define included_hier_CoarseFineBoundary

#include "SAMRAI/SAMRAI_config.h"

#include "SAMRAI/tbox/DescribedClass.h"
#include "SAMRAI/tbox/Dimension.h"
#include "SAMRAI/hier/BoxLevel.h"
#include "SAMRAI/hier/PatchHierarchy.h"

namespace SAMRAI {
namespace hier {

/*!
 * @brief Utility class to construct and maintain a description of the
 * coarse-fine boundary between a patch level and a coarser level.
 *
 * A coarse-fine boundary box is a BoundaryBox object, but it is generated
 * differently than a typical boundary box maintained by a patch geometry
 * object.  A boundary box serving as a coarse-fine boundary box describes part
 * of the boundary of a given patch with its next coarser AMR hierarchy level.
 * It does not intersect any other patch on the same level, nor does it lie
 * on a physical domain boundary, except where the physical boundary is
 * periodic and the appropriate continuation of that boundary is part of a
 * coarser patch level.
 *
 * The coarse-fine boundary is typically created from two adjacent
 * hierarchy levels, but the description lives on (refers to the index
 * space of) the finer level.  Since the coarse-fine boundary
 * describes the boundary to the next coarser level, the coarsest
 * level (level zero) has no coarse-fine boundary.
 *
 * Each CoarseFineBoundary object corresponds to one level, so to
 * represent a entire hierarchy, one would need an array or list of
 * such objects.
 */

class CoarseFineBoundary:public tbox::DescribedClass
{
public:
   /*!
    * @brief Construct a CoarseFineBoundary object with no boundary boxes.
    *
    * @param[in] dim Dimension
    */
   explicit CoarseFineBoundary(
      const tbox::Dimension& dim);

   /*!
    * @brief Copy constructor.
    *
    * @param[in] rhs
    */
   CoarseFineBoundary(
      const CoarseFineBoundary& rhs);

   /*!
    * @brief Construct a CoarseFineBoundary object for the specified
    * level in the given patch hierarchy.
    *
    * @note If level number is zero, the coarse-fine boundary will be empty.
    *
    * @param[in] hierarchy
    * @param[in] level_num
    * @param[in] max_ghost_width The ghost width determines the extent of the
    *                            boundary boxes along the level domain boundary,
    *                            similar to regular domain boundary boxes.  Note
    *                            that as in the case of regular boundary boxes,
    *                            each box will always be one cell wide in the
    *                            direction perpendicular to the patch boundary.
    */
   CoarseFineBoundary(
      const PatchHierarchy& hierarchy,
      int level_num,
      const IntVector& max_ghost_width);

   /*!
    * @brief Construct a CoarseFineBoundary object for a specified level.
    *
    * The coarse-fine boundary will be computed using the physical domain
    * as the reference coarser level.  The physical domain is provided to this
    * method as the 'head' level of the mapped_box_level_to_domain Connector.
    *
    * @note If the level covers the entire physical domain, the coarse-fine
    * boundary will be empty.
    *
    * @param[in] level
    * @param[in] mapped_box_level_to_domain
    * @param[in] mapped_box_level_to_self
    * @param[in] max_ghost_width The ghost width determines the extent of the
    *                            boundary boxes along the level domain boundary,
    *                            similar to regular domain boundary boxes.  Note
    *                            that as in the case of regular boundary boxes,
    *                            each box will always be one cell wide in the
    *                            direction perpendicular to the patch boundary.
    */
   CoarseFineBoundary(
      const PatchLevel& level,
      const Connector& mapped_box_level_to_domain,
      const Connector& mapped_box_level_to_self,
      const IntVector& max_ghost_width);

   /*!
    * @brief Destructor.
    */
   ~CoarseFineBoundary();

   /*!
    * @brief Clear all boundary data or clear boundary data for a given block
    * for multiblock.
    *
    * @param[in] block_number  Optional argument for use in multiblock.
    */
   void
   clear();

   //@{
   /*!
    * @name Functions to get the computed coarse-fine boundaries.
    */

   /*!
    * @brief Get an array of boundary boxes of a given type
    * for a specified patch.
    *
    * The specified patch must exist in the level used to compute
    * the internal state or it is an error.
    *
    * @param[in] global_id
    * @param[in] boundary_type Codimension of boundaries.
    * @param[in] block_num     Defaults to 0 for the single block case
    */
   const tbox::Array<BoundaryBox>&
   getBoundaries(
      const GlobalId& global_id,
      const int boundary_type,
      const BlockId& block_id = BlockId::zero()) const;

   /*!
    * @brief Get an array of node boundary boxes for a specified patch.
    *
    * @see BoundaryBox for more information.
    *
    * The specified patch must exist in the level used to compute
    * the internal state or it is an error.
    *
    * @param[in] global_id
    * @param[in] block_num     Defaults to 0 for the single block case
    */
   const tbox::Array<BoundaryBox>&
   getNodeBoundaries(
      const GlobalId& global_id,
      const BlockId& block_id = BlockId::zero()) const;

   /*!
    * @brief Get an array of edge boundary boxes for a specified patch.
    *
    * @see hier::BoundaryBox for more information.
    *
    * Note that edge boxes are only meaningful if the dimension is > 1.
    * The specified patch must exist in the level used to compute
    * the internal state or it is an error.
    *
    * @param[in] global_id
    * @param[in] block_num     Defaults to 0 for the single block case
    */
   const tbox::Array<BoundaryBox>&
   getEdgeBoundaries(
      const GlobalId& global_id,
      const BlockId& block_id = BlockId::zero()) const;

   /*!
    * @brief Get an array of face boundary boxes for a specified patch.
    *
    * @see hier::BoundaryBox for more information.
    *
    * Note that face boxes are only meaningful if the dimension is > 2.
    * The specified patch must exist in the level used to compute
    * the internal state or it is an error.
    *
    * @param[in] global_id
    * @param[in] block_num     Defaults to 0 for the single block case
    */
   const tbox::Array<BoundaryBox>&
   getFaceBoundaries(
      const GlobalId& global_id,
      const BlockId& block_id = BlockId::zero()) const;

   //@}

   /*!
    * @brief Print out class data.
    *
    * @param[in] os Output stream
    */
   virtual void
   printClassData(
      std::ostream& os) const;

   /*!
    * @brief Assignment operator.
    *
    * @param[in] rhs
    */
   CoarseFineBoundary&
   operator = (
      const CoarseFineBoundary& rhs);

private:
   /* Don't allow default ctor */
   CoarseFineBoundary();

   /*!
    * @brief Compute a CoarseFineBoundary object for a specified level.
    *
    * The coarse-fine boundary will be computed using the physical domain
    * as the reference coarser level.  The physical domain is provided to this
    * method as the 'head' level of the mapped_box_level_to_domain Connector.
    *
    * @note If the level covers the entire physical domain, the coarse-fine
    * boundary will be empty.
    *
    * @param[in] level
    * @param[in] mapped_box_level_to_domain
    * @param[in] mapped_box_level_to_self
    * @param[in] max_ghost_width The ghost width determines the extent of the
    *                            boundary boxes along the level domain boundary,
    *                            similar to regular domain boundary boxes.  Note
    *                            that as in the case of regular boundary boxes,
    *                            each box will always be one cell wide in the
    *                            direction perpendicular to the patch boundary.
    */
   void
   computeFromLevel(
      const PatchLevel& level,
      const Connector& mapped_box_level_to_domain,
      const Connector& mapped_box_level_to_self,
      const IntVector& max_ghost_width);

   /*!
    * @brief Compute a CoarseFineBoundary object for a specified level.
    *
    * The coarse-fine boundary will be computed using the physical domain
    * as the reference coarser level.
    *
    * @note If the level covers the entire physical domain, the coarse-fine
    * boundary will be empty.
    *
    * @param[in] level
    * @param[in] level0
    * @param[in] max_ghost_width The ghost width determines the extent of the
    *                            boundary boxes along the level domain boundary,
    *                            similar to regular domain boundary boxes.  Note
    *                            that as in the case of regular boundary boxes,
    *                            each box will always be one cell wide in the
    *                            direction perpendicular to the patch boundary.
    */
   void
   computeFromLevel(
      const PatchLevel& level,
      const PatchLevel& level0,
      const IntVector& max_ghost_width);

   /*!
    * @brief Dimension of the object.
    */
   const tbox::Dimension d_dim;

   /*!
    * @brief Whether the boundary boxes have been computed.  One entry on the
    * vector for each block in a multiblock grid.
    */
   std::vector<bool> d_initialized;

   /*!
    * @brief Patch boundary boxes describing the coarse-fine boundary.
    *
    * Map each local patch to a PatchBoundaries object.
    */
   std::map<BoxId, PatchBoundaries> d_boundary_boxes;

};

}
}

#endif
