//
// VMime library (http://www.vmime.org)
// Copyright (C) 2002-2013 Vincent Richard <vincent@vmime.org>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 3 of
// the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
// Linking this library statically or dynamically with other modules is making
// a combined work based on this library.  Thus, the terms and conditions of
// the GNU General Public License cover the whole combination.
//

#ifndef VMIME_NET_IMAP_IMAPMESSAGE_HPP_INCLUDED
#define VMIME_NET_IMAP_IMAPMESSAGE_HPP_INCLUDED


#include "vmime/config.hpp"


#if VMIME_HAVE_MESSAGING_FEATURES && VMIME_HAVE_MESSAGING_PROTO_IMAP


#include "vmime/net/message.hpp"
#include "vmime/net/folder.hpp"

#include "vmime/net/imap/IMAPParser.hpp"


namespace vmime {
namespace net {
namespace imap {


class IMAPFolder;


/** IMAP message implementation.
  */

class VMIME_EXPORT IMAPMessage : public message
{
private:

	friend class IMAPFolder;
	friend class IMAPMessagePartContentHandler;
	friend class vmime::creator;  // vmime::create <IMAPMessage>

	IMAPMessage(ref <IMAPFolder> folder, const int num);
	IMAPMessage(ref <IMAPFolder> folder, const int num, const uid& uniqueId);
	IMAPMessage(const IMAPMessage&) : message() { }

	~IMAPMessage();

public:

	int getNumber() const;

	const uid getUniqueId() const;

	int getSize() const;

	bool isExpunged() const;

	ref <const structure> getStructure() const;
	ref <structure> getStructure();

	ref <const header> getHeader() const;

	int getFlags() const;
	void setFlags(const int flags, const int mode = FLAG_MODE_SET);

	void extract(utility::outputStream& os, utility::progressListener* progress = NULL, const int start = 0, const int length = -1, const bool peek = false) const;
	void extractPart(ref <const part> p, utility::outputStream& os, utility::progressListener* progress = NULL, const int start = 0, const int length = -1, const bool peek = false) const;

	void fetchPartHeader(ref <part> p);

	ref <vmime::message> getParsedMessage();

private:

	void fetch(ref <IMAPFolder> folder, const int options);

	void processFetchResponse(const int options, const IMAPParser::message_data* msgData);

	/** Recursively fetch part header for all parts in the structure.
	  *
	  * @param str structure for which to fetch parts headers
	  */
	void fetchPartHeaderForStructure(ref <structure> str);

	/** Recursively contruct parsed message from structure.
	  * Called by getParsedMessage().
	  *
	  * @param parentPart root body part (the message)
	  * @param str structure for which to construct part
	  * @param level current nesting level (0 is root)
	  */
	void constructParsedMessage(ref <bodyPart> parentPart, ref <structure> str, int level = 0);


	enum ExtractFlags
	{
		EXTRACT_HEADER = 0x1,
		EXTRACT_BODY = 0x2,
		EXTRACT_PEEK = 0x10
	};

	void extractImpl(ref <const part> p, utility::outputStream& os, utility::progressListener* progress,
		const int start, const int length, const int extractFlags) const;


	ref <header> getOrCreateHeader();


	void onFolderClosed();

	weak_ref <IMAPFolder> m_folder;

	int m_num;
	int m_size;
	int m_flags;
	bool m_expunged;
	uid m_uid;

	ref <header> m_header;
	ref <structure> m_structure;
};


} // imap
} // net
} // vmime


#endif // VMIME_HAVE_MESSAGING_FEATURES && VMIME_HAVE_MESSAGING_PROTO_IMAP

#endif // VMIME_NET_IMAP_IMAPMESSAGE_HPP_INCLUDED
