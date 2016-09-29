/*
 * Copyright (c) 1994, 2003, Oracle and/or its affiliates. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.  Oracle designates this
 * particular file as subject to the "Classpath" exception as provided
 * by Oracle in the LICENSE file that accompanied this code.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 * or visit www.oracle.com if you need additional information or have any
 * questions.
 */

package sun.tools.asm;

import sun.tools.java.*;
import sun.tools.tree.StringExpression;
import java.io.IOException;
import java.io.DataOutputStream;

/**
 * This is a string expression constant. This constant
 * represents an Java string constant.
 *
 * WARNING: The contents of this source file are not part of any
 * supported API.  Code that depends on them does so at its own risk:
 * they are subject to change or removal without notice.
 */
final
class StringExpressionConstantData extends ConstantPoolData {
    StringExpression str;

    /**
     * Constructor
     */
    StringExpressionConstantData(ConstantPool tab, StringExpression str) {
        this.str = str;
        tab.put(str.getValue());
    }

    /**
     * Write the constant to the output stream
     */
    void write(Environment env, DataOutputStream out, ConstantPool tab) throws IOException {
        out.writeByte(CONSTANT_STRING);
        out.writeShort(tab.index(str.getValue()));
    }

    /**
     * Return the order of the constant
     */
    int order() {
        return 0;
    }

    /**
     * toString
     */
    public String toString() {
        return "StringExpressionConstantData[" + str.getValue() + "]=" + str.getValue().hashCode();
    }
}
