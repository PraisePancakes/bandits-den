#pragma once
#include <iostream>
#include "common.hpp"
#include "lexer.hpp"

namespace Serialization {
	namespace CircFormat {

		// formatter assumes correct syntax in input serialized string
		class CircFormatFunctor {

			std::string get_space_fmt() const noexcept {
				std::string spaces = "";
				for (size_t i = 0; i < depth; i++) {
					spaces += ' ';
				}
				return spaces;
			}

			std::string fmt_obj() noexcept {
				std::string ret = advance().word;
				ret += '\n';
				while (peek().t != TOK_RCURL) {
					ret += fmt_var();
				}
				depth--;
				return ret;
			}

			std::string fmt_array() noexcept  {
				std::string ret = advance().word;

				while (peek().t != TOK_RBRAC) {
					switch (peek().t) {
					case TOK_COMMA:
						ret += advance().word;
						ret += ' ';
						break;
					case TOK_NUM:
					case TOK_STRING:
						ret += advance().word;
						break;
					case TOK_LBRAC:
						ret += '\n';
						ret += fmt_array();
						break;
					default:break;
					}
				}

				ret += advance().word;
				
				return ret;
			}

			std::string fmt_var() noexcept  {
				std::string ret = "";
				ret += get_space_fmt();
				//$key
				ret += advance().word;
				ret += advance().word;
				// : 
				ret += ' ';
				ret += advance().word;
				ret += ' ';
				switch (peek().t) {
				case TOK_STRING:
				case TOK_NUM:
					ret += advance().word;
					break;
				case TOK_LCURL:
					depth++;
					ret += fmt_obj();
					ret += get_space_fmt();
					ret += advance().word;
					break;
				case TOK_LBRAC:
					ret += fmt_array();
					break;
				default:break;
				}

				ret += advance().word;
				ret += '\n';

				return ret;
			};

			inline std::string do_circ_fmt() noexcept {
				std::string fmt = "";
				while (!is_end()) {
					fmt += fmt_var();
				}

				return fmt;
			}

			[[nodiscard]] bool is_end() const noexcept {
				return cursor >= toks.size() - 1;
			}

			Token peek_next()  const noexcept {
				if (is_end()) {
					return toks[cursor];
				}
				return toks[cursor + 1];
			}

			Token prev() const noexcept {
				if (cursor == 0) {
					return toks[cursor];
				}
				return toks[cursor - 1];
			}

			Token peek() const noexcept {
				return toks[cursor];
			}

			Token advance() noexcept {
				if (is_end()) {
					return toks[cursor];
				}
				cursor++;
				return toks[cursor - 1];
			};


		public:
			unsigned short depth = 0;
			size_t cursor = 0;
			TokenVector toks;

			inline std::string operator()(const TokenVector& ref_to_fmt) {
				toks = ref_to_fmt;
				return do_circ_fmt();
			};
		};

		/*
		*	The formatter will take a serialized representation (string) of the Circ file and simply format it.
		*/
		template<typename FunctType = CircFormatFunctor>
		class FormatTypeFunctor {
		public:
			inline std::string operator()(const TokenVector& ref_to_fmt)  {
				FunctType t;
				return t(ref_to_fmt);
			};
		};
	}
}