/*
 * Copyright (c) 2020, Infosys Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
 /******************************************************************************
 *
 * This is an auto generated file.
 * Please do not edit this file.
 * All edits to be made through template source file
 * <TOP-DIR/scripts/GtpV2StackCodeGen/tts/ietemplate.h.tt>
 ******************************************************************************/
#ifndef PDNTYPEIE_H_
#define PDNTYPEIE_H_

#include "manual/gtpV2Ie.h"



class PdnTypeIe: public GtpV2Ie {
public:
    PdnTypeIe();
    virtual ~PdnTypeIe();

    bool encodePdnTypeIe(MsgBuffer &buffer,
                 PdnTypeIeData const &data);
    bool decodePdnTypeIe(MsgBuffer &buffer,
                 PdnTypeIeData &data, Uint16 length);
    void displayPdnTypeIe_v(PdnTypeIeData const &data,
                 Debug &stream);
};

#endif /* PDNTYPEIE_H_ */
