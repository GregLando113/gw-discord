import zipfile as z
import datetime as d

t = d.datetime.now().isoformat(timespec='seconds').replace(':','-')
archive = z.ZipFile('gw-discord-{0}.zip'.format(t), 'w')
archive.write('Release/d3d9.dll','d3d9.dll')
archive.write('Release/gw-discord.dll','bin/gw-discord.dll')
archive.comment = b'gw-discord, Guild Wars integration for Discord by KAOSTheory'
archive.close()
