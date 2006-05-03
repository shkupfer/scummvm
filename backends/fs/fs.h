/* ScummVM - Scumm Interpreter
 * Copyright (C) 2002-2006 The ScummVM project
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL$
 * $Id$
 */

#ifndef BACKENDS_FS_H
#define BACKENDS_FS_H

/*
 * The API described in this header is meant to allow for file system browsing in a
 * portable fashions. To this ends, multiple or single roots have to be supported
 * (compare Unix with a single root, Windows with multiple roots C:, D:, ...).
 *
 * To this end, we abstract away from paths; implementations can be based on
 * paths (and it's left to them whether / or \ or : is the path separator :-);
 * but it is also possible to use inodes or vrefs (MacOS 9) or anything else.
 *
 * NOTE: Backends still have to provide a way to extract a path from a FSIntern
 *
 * You may ask now: "isn't this cheating? Why do we go through all this when we use
 * a path in the end anyway?!?".
 * Well, for once as long as we don't provide our own file open/read/write API, we
 * still have to use fopen(). Since all our targets already support fopen(), it should
 * be possible to get a fopen() compatible string for any file system node.
 *
 * Secondly, with this abstraction layer, we still avoid a lot of complications based on
 * differences in FS roots, different path separators, or even systems with no real
 * paths (MacOS 9 doesn't even have the notion of a "current directory").
 * And if we ever want to support devices with no FS in the classical sense (Palm...),
 * we can build upon this.
 */

/*
 * TODO - Instead of starting with getRoot(), we should rather add a getDefaultDir()
 * call that on Unix might return the current dir or the users home dir...
 * i.e. the root dir is usually not the best starting point for browsing.
 */

#include "common/array.h"
#include "common/str.h"

class FilesystemNode;
class AbstractFilesystemNode;


/**
 * List of multiple file system nodes. E.g. the contents of a given directory.
 * This is subclass instead of just a typedef so that we can use forward
 * declarations of it in other places.
 */
class FSList : public Common::Array<FilesystemNode> {};


class FilesystemNode {
	friend class AbstractFilesystemNode;

	typedef Common::String String;
private:
	AbstractFilesystemNode *_realNode;
	int *_refCount;

	FilesystemNode(AbstractFilesystemNode *realNode);

	/**
	 * Returns a special node representing the FS root. The starting point for
	 * any file system browsing.
	 * On Unix, this will be simply the node for / (the root directory).
	 * On Windows, it will be a special node which "contains" all drives (C:, D:, E:).
	 */
	static AbstractFilesystemNode *getRoot();

	/**
	 * Construct a node based on a path; the path is in the same format as it
	 * would be for calls to fopen().
	 *
	 * I.e. getNodeForPath(oldNode.path()) should create a new node identical to oldNode.
	 *
	 * @TODO: This is of course a place where non-portable code easily will sneak
	 *        in, because the format of the path used here is not well-defined.
	 *        So we really should reconsider this API and try to come up with
	 *        something which is more portable but still flexible enough for our
	 *        purposes.
	 */
	static AbstractFilesystemNode *getNodeForPath(const String &path);


public:
	/**
	 * Flag to tell listDir() which kind of files to list.
	 */
	enum ListMode {
		kListFilesOnly = 1,
		kListDirectoriesOnly = 2,
		kListAll = 3
	};


	/**
	 * Create a new FilesystemNode refering to the specified path. This is
	 * the counterpart to the path() method.
	 */
	FilesystemNode(const String &path);


	FilesystemNode();
	FilesystemNode(const FilesystemNode &node);
	virtual ~FilesystemNode();

	FilesystemNode &operator  =(const FilesystemNode &node);

	/**
	 * Get the parent node of this node. If this node has no parent node,
	 * then it returns a duplicate of this node.
	 */
	FilesystemNode getParent() const;

	/**
	 * Fetch a child node of this node, with the given name. Only valid for
	 * directory nodes (an assertion is triggered otherwise). If no no child
	 * node with the given name exists, an invalid node is returned.
	 */
	FilesystemNode getChild(const String &name) const;

	virtual FSList listDir(ListMode mode = kListDirectoriesOnly) const;
	virtual String displayName() const;
	virtual bool isValid() const;
	virtual bool isDirectory() const;
	virtual String path() const;

	/**
	 * Compare the name of this node to the name of another. Directories
	 * go before normal files.
	 */
	bool operator< (const FilesystemNode& node) const;

protected:
	void decRefCount();
};


#endif
