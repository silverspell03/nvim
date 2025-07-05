-- treesitter: installation & mise à jour des parsers
return  {
    'nvim-treesitter/nvim-treesitter',
    version = false,              -- toujours la dernière version
    build = ':TSUpdate',          -- met à jour les parsers après installation
    opts = {
      -- liste des langages dont on veut le support Treesitter
      ensure_installed = { 'lua', 'vim', 'javascript', 'python', 'bash', 'json' },
      highlight = { enable = true },
      indent    = { enable = true },
    },
    config = function(_, opts)
      require('nvim-treesitter.configs').setup(opts)
    end,
  }
