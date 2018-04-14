#!/bin/sh

#shBegin

APPNAME=wolf3d
EXECPATH=/usr/bin/wolf3d+touch
APPPATH=/usr/share/wolf3d+touch
DEFAULTPATH=${APPPATH}/${APPNAME}

#find data package from user to internal.
if [ -d /home/user/MyDocs/.${APPNAME} ]; then
	DATAPATH=/home/user/MyDocs/.${APPNAME}
else

	if [ -d /home/user/MyDocs/${APPNAME} ]; then
		DATAPATH=/home/user/MyDocs/${APPNAME}
	else

		if [ -d /home/user/${APPNAME} ]; then
			DATAPATH=/home/user/${APPNAME}
		else

			if [ -d /home/user/.${APPNAME} ]; then
				DATAPATH=/home/user/.${APPNAME}
			else

				DATAPATH=${DEFAULTPATH}
			fi
		fi
	fi
fi

#execute
echo "cd ${DATAPATH}"
cd ${DATAPATH}

echo "${APPPATH}/keyon"
${APPPATH}/keyon

echo "${EXECPATH}"
${EXECPATH}

echo "${APPPATH}/keyoff"
${APPPATH}/keyoff

#shEnd
