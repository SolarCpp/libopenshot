/**
 * @file
 * @brief Source file for RendererBase class
 * @author Duzy Chan <code@duzy.info>
 *
 * @section LICENSE
 *
 * Copyright (c) 2008-2014 OpenShot Studios, LLC
 * <http://www.openshotstudios.com/>. This file is part of
 * OpenShot Library (libopenshot), an open-source project dedicated to
 * delivering high quality video editing and animation solutions to the
 * world. For more information visit <http://www.openshot.org/>.
 *
 * OpenShot Library (libopenshot) is free software: you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * OpenShot Library (libopenshot) is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with OpenShot Library. If not, see <http://www.gnu.org/licenses/>.
 */

#include "../include/RendererBase.h"
#include "../include/Frame.h"
#include <stdlib.h> // for realloc

using namespace openshot;

RendererBase::RendererBase() : buffer(NULL)
{
}

RendererBase::~RendererBase()
{
}

void RendererBase::paint(const std::tr1::shared_ptr<Frame> & frame)
{
    const int BPP = 3;
    const tr1::shared_ptr<Magick::Image> image = frame->GetImage();
    const std::size_t width = image->columns();
    const std::size_t height = image->rows();
    const std::size_t bufferSize = width * height * BPP;
    /// Use realloc for fast memory allocation.    
    /// TODO: consider locking the buffer for mt safety
    buffer = reinterpret_cast<unsigned char*>(realloc(buffer, bufferSize));

#if false
    // Not sure if this is actually faster... but it works now
    image->getPixels(0,0, width, height); // load pixels into cache
    image->depth( 8 ); // this is required or it crashes
    image->writePixels(Magick::RGBQuantum, buffer); // write pixel data to our buffer
#else
    // Iterate through the pixel packets, and load our own buffer
	// Each color needs to be scaled to 8 bit (using the ImageMagick built-in ScaleQuantumToChar function)
    const Magick::PixelPacket *pixels = frame->GetPixels();
    for (int n = 0, i = 0; n < width * height; n += 1, i += 3) {
		buffer[i+0] = MagickCore::ScaleQuantumToChar((Magick::Quantum) pixels[n].red);
		buffer[i+1] = MagickCore::ScaleQuantumToChar((Magick::Quantum) pixels[n].green);
		buffer[i+2] = MagickCore::ScaleQuantumToChar((Magick::Quantum) pixels[n].blue);
    }
#endif

    this->render(RGB_888, width, height, width * BPP, buffer);
}
