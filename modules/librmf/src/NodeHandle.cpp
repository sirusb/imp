/**
 *  \file RMF/Category.h
 *  \brief Handle read/write of Model data from/to files.
 *
 *  Copyright 2007-2012 IMP Inventors. All rights reserved.
 *
 */

#include <RMF/NodeHandle.h>
#include <boost/tuple/tuple.hpp>
#include <RMF/Category.h>
#include <RMF/FileHandle.h>
#include <RMF/decorators.h>

namespace RMF {

NodeHandle::NodeHandle(int node, internal::SharedData *shared):
    NodeConstHandle(node, shared) {
}

NodeHandle NodeHandle::add_child(std::string name, NodeType t) {
  return NodeHandle(get_shared_data()->add_child(get_node_id(), name, t),
                    get_shared_data());
}


FileHandle NodeHandle::get_file() const {
  return FileHandle(get_shared_data());
}

vector<NodeHandle> NodeHandle::get_children() const {
  Ints children= get_shared_data()->get_children(get_node_id());
  vector<NodeHandle> ret(children.size());
  for (unsigned int i=0; i< ret.size(); ++i) {
    ret[i]= NodeHandle(children[ret.size()-i-1], get_shared_data());
  }
  return ret;
}

NodeHandles get_children_resolving_aliases(NodeHandle nh) {
  StaticAliasFactory saf(nh.get_file());
  NodeHandles ret= nh.get_children();
  for (unsigned int i=0; i< ret.size(); ++i) {
    if (ret[i].get_type()== ALIAS && saf.get_is(ret[i])) {
      ret[i]= nh.get_file().get_node_from_id(saf.get(ret[i]).get_alias());
    }
  }
  return ret;
}

NodeHandle add_child_alias(NodeHandle parent,
               NodeHandle alias) {
  NodeHandle nh=parent.add_child(alias.get_name() + " alias",
                                 ALIAS);
  StaticAliasFactory saf(parent.get_file());
  saf.get(nh).set_alias(alias.get_id());
  return nh;
}


} /* namespace RMF */
