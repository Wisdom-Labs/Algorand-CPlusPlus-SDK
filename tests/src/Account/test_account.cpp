//
// Created by ubuntu on 10/4/22.
//

#include "CppUTest/MemoryLeakDetectorMallocMacros.h"
#include "CppUTest/MemoryLeakDetectorNewMacros.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "vertices.h"
#include <vertices_log.h>
#include "unix_config.h"
#include <string.h>
#include <sodium.h>
#include <base32.h>


static provider_info_t providers =
        {.url = (char *) SERVER_URL, .port = SERVER_PORT, .header = (char *) SERVER_TOKEN_HEADER};

static ret_code_t
vertices_evt_handler(vtc_evt_t *evt);

static vertex_t m_vertex = {
        .provider = &providers,
        .vertices_evt_handler = vertices_evt_handler
};

static ret_code_t
vertices_evt_handler(vtc_evt_t *evt)
{
    ret_code_t err_code = VTC_SUCCESS;

    return err_code;
}


typedef struct
{
    unsigned char private_key[ADDRESS_LENGTH];  //!< 32-bytes private key
    account_info_t *vtc_account;               //!< pointer to Vertices account data
} account_t;

// Alice's account is used to send data, keys will be retrived from config/key_files.txt
static account_t alice_account = {.private_key = {0}, .vtc_account = NULL};

/// Create new random account
/// Account keys will be stored in files
static ret_code_t
create_new_account(void)
{
    ret_code_t err_code;

    unsigned char seed[crypto_sign_ed25519_SEEDBYTES] = {0};
    unsigned char ed25519_pk[crypto_sign_ed25519_PUBLICKEYBYTES];

    LOG_WARNING("🧾 Creating new random account and storing it (path " CONFIG_PATH ")");

    unsigned char ed25519_sk[crypto_sign_ed25519_SECRETKEYBYTES];
    randombytes_buf(seed, sizeof(seed));

    crypto_sign_ed25519_seed_keypair(ed25519_pk, ed25519_sk, seed);

    memcpy(alice_account.private_key, ed25519_sk, sizeof(alice_account.private_key));

    FILE *fw_priv = fopen(CONFIG_PATH "private_key.bin", "wb");
    if (fw_priv == NULL)
    {
        LOG_ERROR("Cannot create " CONFIG_PATH "private_key.bin");
        return VTC_ERROR_NOT_FOUND;
    }
    else
    {
        fwrite(ed25519_sk, 1, ADDRESS_LENGTH, fw_priv);
        fclose(fw_priv);
    }

    // adding account, account address will be computed from binary public key
    err_code = vertices_account_new_from_bin((char *) ed25519_pk, &alice_account.vtc_account);

    // we can now store the b32 address in a file
    FILE *fw_pub = fopen(CONFIG_PATH "public_b32.txt", "w");
    if (fw_pub != NULL)
    {
        size_t len = strlen(alice_account.vtc_account->public_b32);

        fwrite(alice_account.vtc_account->public_b32, 1, len, fw_pub);
        fwrite("\n", 1, 1, fw_pub);
        fclose(fw_pub);
    }

    return err_code;
}

static ret_code_t
load_existing_account()
{
    ret_code_t err_code;

    char public_b32[PUBLIC_B32_STR_MAX_LENGTH] = {0};

    size_t bytes_read = 0;

    // we either create a new random account or load it from private and public key files.
    // key files can also be generated using [`algokey`](https://developer.algorand.org/docs/reference/cli/algokey/generate/)
    FILE *f_priv = fopen(CONFIG_PATH "private_key.bin", "rb");
    if (f_priv != NULL)
    {
        LOG_INFO("🔑 Loading private key from: %s", CONFIG_PATH "private_key.bin");

        bytes_read = fread(alice_account.private_key, 1, ADDRESS_LENGTH, f_priv);
        fclose(f_priv);
    }

    if (f_priv == NULL || bytes_read != ADDRESS_LENGTH)
    {
        LOG_WARNING(
                "🤔 private_key.bin does not exist or keys not found. You can pass the -n flag to create a new account");

        return VTC_ERROR_NOT_FOUND;
    }

    FILE *f_pub = fopen(CONFIG_PATH "public_b32.txt", "r");
    bytes_read = 0;
    if (f_pub != NULL)
    {
        LOG_INFO("🔑 Loading public key from: %s", CONFIG_PATH "public_b32.txt");

        bytes_read = fread(public_b32, 1, PUBLIC_B32_STR_MAX_LENGTH, f_pub);
        fclose(f_pub);

        size_t len = strlen(public_b32);
        while (public_b32[len - 1] == '\n' || public_b32[len - 1] == '\r')
        {
            public_b32[len - 1] = '\0';
            len--;
        }
    }

    if (f_pub == NULL || bytes_read < ADDRESS_LENGTH)
    {
        LOG_WARNING(
                "🤔 public_b32.txt does not exist or keys not found. You can pass the -n flag to create a new account");

        return VTC_ERROR_NOT_FOUND;
    }

    err_code = vertices_account_new_from_b32(public_b32, &alice_account.vtc_account);
    VTC_ASSERT(err_code);

    LOG_INFO("💳 Created Alice's account: %s", alice_account.vtc_account->public_b32);

    return VTC_SUCCESS;
}

TEST_GROUP(Account){
        void setup() override {
            ret_code_t err_code;

            int ret = sodium_init();

            if(ret == 1){
                LOG_DEBUG("Sodium library was installed already.");
            }
            else
            {
                LOG_INFO("Sodium library has been installed successfully.");
            }

            LONGS_EQUAL(0, ret);

            // create new vertex
            err_code = vertices_new(&m_vertex);
            ENUMS_EQUAL_INT(err_code , VTC_SUCCESS);
            LOG_INFO("😎 Created new vertices");
        }
        void teardown() override {}
};

TEST(Account, create_new_account) {
    ret_code_t err_code;

    err_code = create_new_account();

    if(err_code == VTC_SUCCESS){
        LOG_INFO("😎 Created new account.");
    }

    ENUMS_EQUAL_INT(err_code, VTC_SUCCESS);
}

TEST(Account, load_existing_account) {
    ret_code_t err_code;

    err_code = load_existing_account();

    if(err_code == VTC_SUCCESS){
        LOG_INFO("😎 Loaded existing account from file.");
    }

    ENUMS_EQUAL_INT(err_code, VTC_SUCCESS);
}