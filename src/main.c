/*
 * main.c - Entry point for program
 *
 * Copyright (C) 2023 [github.com/nickelulz]
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <log.h>
#include <pdfio.h>
#include <pdfio-content.h>
#include <string.h>

int main()
{
	pdfio_file_t *pdf = pdfioFileOpen("data/hello_world.pdf", NULL, NULL, NULL, NULL);
	
	for (size_t page_index = 0, count = pdfioFileGetNumPages(pdf); page_index < count; ++page_index) 
	{
		pdfio_obj_t *page = pdfioFileGetPage(pdf, page_index); 
		if (page == NULL)
			continue;

		size_t stream_count = pdfioPageGetNumStreams(page);

		for (size_t stream_index = 0; stream_index < stream_count; ++stream_index) 
		{
			pdfio_stream_t *stream = pdfioPageOpenStream(page, stream_index, true); 
			if (stream == NULL)
				continue;

			bool first = true;
			char buffer[4000];

			while (pdfioStreamGetToken(stream, buffer, sizeof(buffer))) 
			{
				log_debug("token: %s", buffer);

				if (buffer[0] == '(') 
				{
					if (first) {
						first = false;
					} else {
						putchar(' ');
					}

					fputs(buffer + 1, stdout);
				}

				else if (!strcmp(buffer, "Td") || 
					!strcmp(buffer, "TD") || 
					!strcmp(buffer, "T*") || 
					!strcmp(buffer, "\'") || 
					!strcmp(buffer, "\"")) 
				{
					putchar('\n');
					first = true;
				}
			}

			if (!first)
				putchar('\n');

			pdfioStreamClose(stream);
		}
	}

	pdfioFileClose(pdf);
	
	return 0;
}