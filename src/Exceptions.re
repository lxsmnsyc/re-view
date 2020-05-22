/**
 * @license
 * MIT License
 *
 * Copyright (c) 2020 Alexis Munsayac
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 *
 * @author Alexis Munsayac <alexis.munsayac@gmail.com>
 * @copyright Alexis Munsayac 2020
 */

/**
 * Unitialized Fiber constructor
 */
exception MissingBasicComponentConstructor;
exception MissingComponentConstructor;
exception MissingMemoComponentConstructor;
exception MissingMemoBasicComponentConstructor;

/**
 * Attempt to call hooks outside component or memo component
 */
exception OutOfContextHookCall;

/**
 * Misplaced hook call
 */
exception IncompatibleHook;

/**
 * Attempt to access context
 */
exception MissingContext;

/**
 * Missing context value
 */
exception DesyncContextValue;

/**
 * Missing context instsance
 */
exception UnboundContextInstance;

/**
 * Attempt to construct a host instance with an invalid constructor.
 */
exception InvalidHostConstructor;
